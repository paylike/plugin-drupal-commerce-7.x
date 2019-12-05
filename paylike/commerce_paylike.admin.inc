<?php

/**
 * Configuration form
 */
function commerce_paylike_settings_form($settings = null) {
  $form['txn_type'] = array(
    '#type' => 'radios',
    '#title' => t('Credit card transaction type'),
    '#description' => t('Will be used to process transactions during checkout.'),
    '#options' => array(
      COMMERCE_CREDIT_AUTH_CAPTURE => t('Instant mode (authorize and capture money all at once)'),
      COMMERCE_CREDIT_AUTH_ONLY => t('Delayed mode (requires manual capture after checkout)'),
    ),
    '#default_value' => isset($settings['txn_type']) ? $settings['txn_type'] : COMMERCE_CREDIT_AUTH_CAPTURE,
  );
  $form['test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#default_value' => isset($settings['test_mode']) ? $settings['test_mode'] : true,
  );
  $form['api_key_test_public'] = array(
    '#type' => 'textfield',
    '#title' => t('Test Public Key'),
    '#default_value' => !empty($settings['api_key_test_public']) ? $settings['api_key_test_public'] : '',
    '#description' => t('Your Development Paylike Public API Key.'),
  );
  $form['api_key_test_private'] = array(
    '#type' => 'textfield',
    '#title' => t('Test Secret Key'),
    '#default_value' => !empty($settings['api_key_test_private']) ? $settings['api_key_test_private'] : '',
    '#description' => t('Your Development Paylike Private API Key. An API key can be obtained by creating a merchant and adding an app through Paylike <a href="@dashboard" target="_blank">dashboard</a>.', array('@dashboard' => 'https://app.paylike.io/')),
  );
  $form['api_key_live_public'] = array(
    '#type' => 'textfield',
    '#title' => t('Live Public Key'),
    '#default_value' => !empty($settings['api_key_live_public']) ? $settings['api_key_live_public'] : '',
    '#description' => t('Your Live Paylike Public API Key.'),
  );
  $form['api_key_live_private'] = array(
    '#type' => 'textfield',
    '#title' => t('Live Secret Key'),
    '#default_value' => !empty($settings['api_key_live_private']) ? $settings['api_key_live_private'] : '',
    '#description' => t('Your Live Paylike Private API Key. An API key can be obtained by creating a merchant and adding an app through Paylike <a href="@dashboard" target="_blank">dashboard</a>.', array('@dashboard' => 'https://app.paylike.io/')),
  );
  $form['payment_method_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method title'),
    '#default_value' => !empty($settings['payment_method_title']) ? $settings['payment_method_title'] : '',
    '#description' => t('The title will appear on checkout page in payment methods list. Leave blank for default "Paylike" title.'),
  );
  $form['payment_method_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment method description'),
    '#default_value' => !empty($settings['payment_method_description']) ? $settings['payment_method_description'] : '',
    '#description' => t('The description will appear on checkout page.'),
  );
  $form['popup_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment popup title'),
    '#default_value' => !empty($settings['popup_title']) ? $settings['popup_title'] : '',
    '#description' => t('The title will appear on the Paylike payment popup window. Leave blank to show the site name.'),
  );
  return $form;
}

/**
 * Validation function and normalize keys (trim spaces)
 *
 * @param $form
 * @param $form_state
 */
function commerce_paylike_settings_form_validate($form, &$form_state) {
  $elements = array('commerce_paylike_api_key_test_private', 'commerce_paylike_api_key_test_public',
    'commerce_paylike_api_key_live_private', 'commerce_paylike_api_key_live_public');

  if ($form_state['values']['uc_pg_commerce_paylike_enabled']) {
    foreach ($elements as $element_name) {
      $form_state['values'][$element_name] = _commerce_paylike_sanitize_key($form_state['values'][$element_name]);
      if (!_commerce_paylike_validate_key($form_state['values'][$element_name])) {
        form_set_error($element_name, t('@name does not appear to be a valid Paylike key', array('@name' => $element_name)));
      }
    }
  }

  // Make sure they haven't tried to validate credit card numbers, as commerce_paylike will not provide a real one.
  if (!empty($form_state['values']['uc_credit_validate_numbers'])) {
    form_set_error('uc_credit_validate_numbers', t('When used with Ubercart Paylike, "Validate credit card number at checkout" must be unchecked.'));
  }

}

/**
 * Sanitize and strip whitespace from API keys
 *
 * @param $key
 * @return string
 */
function _commerce_paylike_sanitize_key($key) {
  $key = trim($key);
  $key = check_plain($key);
  return $key;
}

/**
 * Validate API key
 *
 * @param $key
 * @return boolean
 */
function _commerce_paylike_validate_key($key) {
  $valid = preg_match('/^[a-zA-Z0-9_]+$/', $key);
  return $valid;
}


/**
 * Refund form.
 */
function commerce_paylike_refund_form($form, &$form_state, $order, $transaction) {
  // Save transaction and order for future usage
  $form_state['transaction'] = $transaction;
  $form_state['order'] = $order;

  // Calculate the amount left available for a refund.
  $refunded = isset($transaction->data['paylike']['amount_refunded']) ? $transaction->data['paylike']['amount_refunded'] : 0;
  $remaining = commerce_currency_amount_to_decimal($transaction->amount - $refunded, $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter any amount to refund up to @txn_amount', array('@txn_amount' => commerce_currency_format($remaining, $transaction->currency_code, null, false))),
    '#default_value' => $remaining,
    '#required' => true,
    '#size' => 8,
  );
  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process refund'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Refund form validation.
 */
function commerce_paylike_refund_form_validate($form, $form_state) {
  $vals = $form_state['values'];

  if (!is_numeric($vals['amount']) || $vals['amount'] <= 0) {
    form_set_error('', t('Please enter amount to refund.'));
  }
  if (!isset($form_state['transaction']) || !isset($form_state['transaction']->remote_id)) {
    form_set_error('commerce-paylike-refund-form', t('Transaction not found.'));
  }
}

/**
 * Submit handler for refund form.
 */
function commerce_paylike_refund_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Cancel')) {
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
    return;
  }

  try {
    $transaction = $form_state['transaction'];
    $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
    $paylike_transaction = _commerce_paylike_transaction_refund($transaction->remote_id, $amount);
    if (!$paylike_transaction['successful']) throw new Exception($paylike_transaction['error']);

    // Update information about Drupal transaction
    $transaction->data['paylike']['amount_refunded'] = $paylike_transaction['refundedAmount'];
    commerce_payment_transaction_save($transaction);

    // Redirect URL
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
  } catch (Exception $e) {
    drupal_set_message(t('Refund failed. Error: @error', array('@error' => $e->getMessage())), 'error');
  }

  $transaction_new = commerce_payment_transaction_new('commerce_paylike', $transaction->order_id);
  $transaction_new->instance_id = $transaction->instance_id;
  $transaction_new->currency_code = $transaction->currency_code;
  $transaction_new->amount = $amount * -1;
  $transaction_new->data['paylike']['type'] = 'refund';

  // Create a new record about refund
  if ($paylike_transaction['successful']) {
    $transaction_new->message = t('Refund succeeded.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  } else {
    $transaction_new->message =  t('Refund failed.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  commerce_payment_transaction_save($transaction_new);
}

function commerce_paylike_capture_form($form, &$form_state, $order, $transaction) {
  // Save transaction and order for future usage
  $form_state['transaction'] = $transaction;
  $form_state['order'] = $order;

  // Calculate the amount left available for a refund.
  $captured = isset($transaction->data['paylike']['amount_captured']) ? $transaction->data['paylike']['amount_captured'] : 0;
  $remaining = commerce_currency_amount_to_decimal($transaction->amount - $captured, $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount to capture'),
    '#description' => t('Enter any amount to capture up to @txn_amount', array('@txn_amount' => commerce_currency_format($remaining, $transaction->currency_code, null, false))),
    '#default_value' => $remaining,
    '#required' => true,
    '#size' => 8,
  );
  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Capture'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

function commerce_paylike_capture_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Cancel')) {
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
    return;
  }

  try {
    $transaction = $form_state['transaction'];
    $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
    $paylike_transaction = _commerce_paylike_transaction_capture($transaction->remote_id, $amount);
    if (!$paylike_transaction['successful']) throw new Exception($paylike_transaction['error']);

    // Update information about Drupal transaction
    $transaction->data['paylike']['amount_captured'] = $paylike_transaction['capturedAmount'];
    commerce_payment_transaction_save($transaction);

    // Redirect URL
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
  } catch (Exception $e) {
    drupal_set_message(t('Capture failed. Error: @error', array('@error' => $e->getMessage())), 'error');
  }

  $transaction_new = commerce_payment_transaction_new('commerce_paylike', $transaction->order_id);
  $transaction_new->instance_id = $transaction->instance_id;
  $transaction_new->currency_code = $transaction->currency_code;
  $transaction_new->amount = $amount;
  $transaction_new->remote_id = $transaction->remote_id;
  $transaction_new->data['paylike']['type'] = 'payment';

  // Create a new record about capture
  if ($paylike_transaction['successful']) {
    $transaction_new->message = t('Capture succeeded.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  } else {
    $transaction_new->message =  t('Capture failed.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  commerce_payment_transaction_save($transaction_new);
}

function commerce_paylike_void_form($form, &$form_state, $order, $transaction) {
  // Save transaction and order for future usage
  $form_state['transaction'] = $transaction;
  $form_state['order'] = $order;

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process void'),
  );

  return $form;
}

function commerce_paylike_void_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Cancel')) {
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
    return;
  }

  try {
    $transaction = $form_state['transaction'];
    $paylike_transaction = _commerce_paylike_transaction_void($transaction->remote_id, $transaction->amount);
    if (!$paylike_transaction['successful']) throw new Exception($paylike_transaction['error']);

    // Update information about Drupal transaction
    $transaction->data['paylike']['amount_voided'] = $paylike_transaction['voidedAmount'];
    commerce_payment_transaction_save($transaction);

    // Redirect URL
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
  } catch (Exception $e) {
    drupal_set_message(t('Void failed. Error: @error', array('@error' => $e->getMessage())), 'error');
  }

  $transaction_new = commerce_payment_transaction_new('commerce_paylike', $transaction->order_id);
  $transaction_new->instance_id = $transaction->instance_id;
  $transaction_new->currency_code = $transaction->currency_code;
  $transaction_new->amount = $transaction->amount * -1;
  $transaction_new->data['paylike']['type'] = 'void';

  // Create a new record about void
  if ($paylike_transaction['successful']) {
    $transaction_new->message = t('Void succeeded.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  } else {
    $transaction_new->message =  t('Void failed.');
    $transaction_new->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  commerce_payment_transaction_save($transaction_new);
}
