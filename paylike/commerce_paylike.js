(function ($) {
  Drupal.behaviors.commerce_paylike = {
    attach: function (context, settings) {

      // Attach the code only once
      $('.paylike-button', context).once('commerce_paylike', function() {

        function handleResponse(error, response) {
          if (error) {
            return console.log(error);
          }
          console.log(response);
          $('.paylike-button').val(Drupal.t('Change credit card details'));
          $('#paylike_transaction_id').val(response.transaction.id);
          // Hide 'Payment failed' error if payment success
          $('div.fieldset-wrapper').find('div.error').hide();
        }

        $(this).click(function (event) {
          event.preventDefault();
          if (settings.commerce_paylike.public_key === "") {
            $('#payment-details').prepend('<div class="messages error">' + Drupal.t('Configure Paylike settings please') + '</div>');
            return;
          }
          var paylike = Paylike({key : settings.commerce_paylike.public_key}),
            config = settings.commerce_paylike.config;

          paylike.pay(config, handleResponse);
        });
      });

    }
  }
})(jQuery);
