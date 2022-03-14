# Drupal Commerce plugin for Paylike [![Build Status](https://travis-ci.org/paylike/plugin-drupal-commerce-7.x.svg?branch=master)](https://travis-ci.org/paylike/plugin-drupal-commerce-7.x)
This plugin is *not* developed or maintained by Paylike but kindly made
available by a user.

Released under the GPL V3 license: https://opensource.org/licenses/GPL-3.0

## Supported Drupal Commerce versions

[![Last succesfull test](https://log.derikon.ro/api/v1/log/read?tag=drupalcommerce7&view=svg&label=DrupalCommerce&key=ecommerce&background=00b4ff)](https://log.derikon.ro/api/v1/log/read?tag=drupalcommerce7&view=html)

*The plugin has been tested with most versions of Drupal Commerce at every iteration. We recommend using the latest version of Drupal Commerce, but if that is not possible for some reason, test the plugin with your Drupal Commerce version and it would probably function properly.*

Drupal framework version last tested on: 7.78

## Installation

Once you have installed Drupal Commerce on your Drupal setup, follow these simple steps:
  1. Signup at [paylike.io](https://paylike.io) (it’s free)
   1. Create a live account
   1. Create an app key for your Drupal website
   1. Upload the ```paylike.zip``` trough the Drupal Admin
   1. Download and install the Paylike PHP Library version 1.0.4 or newer
          from https://github.com/paylike/php-api/releases. The recommended technique is
          to use the command:

          `drush ldl paylike`

          If you don't use `drush ldl paylike`, download and install the Paylike library in
          `sites/all/libraries/paylike` such that the path to `composer.json`
          is `sites/all/libraries/paylike/composer.json`. YOU MUST CLEAR THE CACHE AFTER
          CHANGING THE PAYLIKE PHP LIBRARY. The Libraries module caches its memory of
          libraries like the Paylike Library.
   1. Activate the plugin through the 'Modules' screen in Drupal.
   1.  Visit your Drupal Commerce Store Administration page, Configuration
       section, and enable the gateway under the Payment methods.
       (admin/commerce/config/payment-methods)
   1. Insert Paylike API keys, from https://app.paylike.io.
       Go to `admin/commerce/config/payment-methods/manage/commerce_payment_commerce_paylike/` and click edit on actions to get to the configuration screen
   1. Select the default credit transaction type. This module supports immediate
              or delayed capture modes. Immediate capture will be done when users confirm
              their orders. In delayed mode administrator should capture the money manually from
              orders administration page (admin/commerce/orders). Select an order under the payment section you will have the capture/void/refund options available based on the current state of the order


## Updating settings

Under the Paylike payment method settings, you can:
 * Update the payment method text in the payment gateways list
 * Update the payment method description in the payment gateways list
 * Update the title that shows up in the payment popup
 * Add test/live keys
 * Set payment mode (test/live)
 * Change the capture type (Instant/Delayed)

 ## How to

 1. Capture
    * In Instant mode, the orders are captured automatically
    * In delayed mode you can capture an order by using the Payment operations from an order. If available the capture operation will show up. (admin/commerce/orders/ORDER_ID/payment)
 2. Refund
    * You can refund an order by using the Payment operations from an order. If available the refund operation will show up. (admin/commerce/orders/ORDER_ID/payment)
 3. Void
    * You can void an order by using the Payment operations from an order. If available the void operation will show up. (admin/commerce/orders/ORDER_ID/payment)

## Available features

1. Capture
   * Drupal admin panel: full/partial capture
   * Paylike admin panel: full/partial capture
2. Refund
   * Drupal admin panel: full/partial refund
   * Paylike admin panel: full/partial refund
3. Void
   * Drupal admin panel: full void
   * Paylike admin panel: full/partial void