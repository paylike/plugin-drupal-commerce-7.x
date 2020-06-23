<?php


namespace DrupalCommerce;

use Facebook\WebDriver\Exception\NoAlertOpenException;
use Facebook\WebDriver\Exception\ElementNotVisibleException;
use Facebook\WebDriver\Exception\UnrecognizedExceptionException;
use Facebook\WebDriver\Exception\NoSuchElementException;
use Facebook\WebDriver\Exception\StaleElementReferenceException;
use Facebook\WebDriver\Exception\TimeOutException;
use Facebook\WebDriver\Exception\UnexpectedTagNameException;
use Facebook\WebDriver\Remote\RemoteWebElement;
use Facebook\WebDriver\WebDriverBy;
use Facebook\WebDriver\WebDriverDimension;
use Facebook\WebDriver\WebDriverExpectedCondition;

class DrupalCommerceRunner extends DrupalCommerceTestHelper
{

    /**
     * @param $args
     *
     * @throws NoSuchElementException
     * @throws TimeOutExceptionDrupalCommerce
     * @throws UnexpectedTagNameException
     */
    public function ready($args) {
        $this->set($args);
        $this->go();
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function loginAdmin() {
        $this->goToPage('', '#edit-name', true);

        while ( ! $this->hasValue('#edit-name', $this->user)) {
            $this->typeLogin();
        }
        $this->click('.form-submit');
        $this->waitForElement('.toolbar-menu');
    }

    /**
     *  Insert user and password on the login screen
     */
    private function typeLogin() {
        $this->type('#edit-name', $this->user);
        $this->type('#edit-pass', $this->pass);
    }

    /**
     * @param $args
     */
    private function set($args) {
        foreach ($args as $key => $val) {
            $name = $key;
            if (isset($this->{$name})) {
                $this->{$name} = $val;
            }
        }
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function changeCurrency() {
        $this->goToPage("commerce/config/currency", "#edit-commerce-default-currency", true);
        $this->selectValue("#edit-commerce-default-currency", "$this->currency");
        $this->click("#edit-submit");
        $this->waitForElement(".status");
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function changeMode() {
        $this->goToPage('commerce/config/payment-methods', '', true);
        $this->click("//a[contains(text(), 'Paylike')]");
        $this->click(".rules-element-label a ");
        $this->click("//label[contains(text(), '" . $this->capture_mode . "')]");
        $this->captureMode();
    }


    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */

    private function logVersionsRemotly() {
        $versions = $this->getVersions();
        $this->wd->get(getenv('REMOTE_LOG_URL') . '&key=' . $this->get_slug($versions['ecommerce']) . '&tag=drupalcommerce7&view=html&' . http_build_query($versions));
        $this->waitForElement('#message');
        $message = $this->getText('#message');
        $this->main_test->assertEquals('Success!', $message, "Remote log failed");
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    private function getVersions() {
        $this->goToPage("modules", "#edit-modules-commerce", "true");
        $drupalcommerce = $this->wd->executeScript("
            var paylikeLabel = document.querySelectorAll('label[for=\"edit-modules-commerce-commerce-enable\"]');
            return paylikeLabel[0].parentNode.nextSibling.innerText;
            "
        ); $paylike = $this->wd->executeScript("
            var paylikeLabel = document.querySelectorAll('label[for=\"edit-modules-commerce-contrib-commerce-paylike-enable\"]');
            return paylikeLabel[0].parentNode.nextSibling.innerText;
            "
        );

        return ['ecommerce' => $drupalcommerce, 'plugin' => $paylike];

    }


    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    private function directPayment() {

        $this->changeCurrency();
        $this->goToPage('', '.node-readmore');
        $this->click(".node-readmore a");
        $this->waitForElement(".commerce-add-to-cart #edit-submit");
        $this->addToCart();
        $this->proceedToCheckout();
        $this->amountVerification();
        $this->finalPaylike();
        $this->selectOrder();
        if ($this->capture_mode == 'Delayed') {
            $this->capture();
        } else {
            $this->refund();
        }

    }


    /**
     * @param $status
     *
     * @throws NoSuchElementException
     * @throws UnexpectedTagNameException
     */


    public function moveOrderToStatus($status) {
        switch ($status) {
            case "Confirmed":
                $selector = ".commerce-payment-transaction-capture a";
                break;
            case "Refunded":
                $selector = ".commerce-payment-transaction-refund a";
                break;
        }
        $this->click($selector);
        $this->waitForElement(".form-item #edit-amount");
        $this->click("#edit-submit");
        $this->waitForElement(".view-content");
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function capture() {
        $this->moveOrderToStatus('Confirmed');
        $messages = $this->getText('.views-row-last .views-field-message');
        $this->main_test->assertEquals('Capture succeeded.', $messages, "Completed");
    }

    /**
     *
     */

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnrecognizedExceptionException
     */
    public function captureMode() {
        $this->click("#edit-submit");
        $this->waitforElement(".status");
    }


    /**
     *
     */
    public function addToCart() {
        $this->click('.commerce-add-to-cart #edit-submit');
        $this->waitForElement('.status');
        $this->click('.status  a');
        $this->waitForElement('.form-actions  #edit-checkout');

    }

    /**
     *
     */
    public function proceedToCheckout() {
        $this->click(".form-actions #edit-checkout");
        $this->waitForElement(".street-block #edit-customer-profile-billing-commerce-customer-address-und-0-thoroughfare");
        $this->type("#edit-customer-profile-billing-commerce-customer-address-und-0-name-line", "John Doe");
        $this->type("#edit-customer-profile-billing-commerce-customer-address-und-0-thoroughfare", "First Street");
        $this->type("#edit-customer-profile-billing-commerce-customer-address-und-0-locality", "New York");
        $this->type("#edit-customer-profile-billing-commerce-customer-address-und-0-postal-code", "12351");
        $this->click(".checkout-continue");
        $this->waitForElement(".commerce_paylike-processed");

    }

    /**
     *
     */
    public function amountVerification() {
        $amount         =  $amount         = $this->wd->executeScript("return window.Drupal.settings.commerce_paylike.config.amount");
        $expectedAmount = $this->getText('.commerce-price-formatted-components .component-total');
        $expectedAmount = preg_replace("/[^0-9.]/", "", $expectedAmount);
        $expectedAmount = trim($expectedAmount, '.');
        $expectedAmount = ceil(round($expectedAmount, 4) * get_paylike_currency_multiplier($this->currency));
        $this->main_test->assertEquals($expectedAmount, $amount, "Checking minor amount for " . $this->currency);

    }


    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function finalPaylike() {
        $this->waitForElement('.paylike-button');
        $this->click('.paylike-button');
        $this->popupPaylike();
        $this->waitForElement("#edit-continue");
        $this->waitElementDisappear('.paylike.overlay');
        $this->click("#edit-continue");
        $completedValue = $this->getText("#page-title");
        // because the title of the page matches the checkout title, we need to use the order received class on body
        $this->main_test->assertEquals('Checkout complete', $completedValue);
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function popupPaylike() {
        try {
            $this->type('.paylike.overlay .payment form #card-number', 41000000000000);
            $this->type('.paylike.overlay .payment form #card-expiry', '11/22');
            $this->type('.paylike.overlay .payment form #card-code', '122');
            $this->click('.paylike.overlay .payment form button');
        } catch (NoSuchElementException $exception) {
            $this->confirmOrder();
            $this->popupPaylike();
        }

    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function selectOrder() {
        $this->goToPage("commerce/orders", ".commerce-order-edit", true);
        $this->click(".commerce-order-payment a");
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function refund() {
        $this->moveOrderToStatus('Refunded');
        $messages = $this->getText('.views-row-last .views-field-message');
        $this->main_test->assertEquals('Refund succeeded.', $messages, "Refunded");
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    public function confirmOrder() {
        $this->waitForElement('#paylike-payment-button');
        $this->click('#paylike-payment-button');
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     */
    private function settings() {
        $this->changeMode();
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    private function go() {
        $this->changeWindow();
        $this->loginAdmin();
        if ($this->log_version) {
            $this->logVersionsRemotly();

            return $this;
        }

        $this->settings();
        $this->directPayment();

    }

    /**
     *
     */
    private function changeWindow() {
        $this->wd->manage()->window()->setSize(new WebDriverDimension(1600, 1024));
    }


}

