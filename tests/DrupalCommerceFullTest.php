<?php

namespace DrupalCommerce;


use Facebook\WebDriver\Exception\NoSuchElementException;
use Facebook\WebDriver\Exception\TimeOutException;
use Facebook\WebDriver\Exception\UnexpectedTagNameException;
use Facebook\WebDriver\WebDriver;
use Facebook\WebDriver\WebDriverBy;
use Facebook\WebDriver\WebDriverExpectedCondition;
use Lmc\Steward\Test\AbstractTestCase;

/**
 * @group drupalcommerce_full_test
 */
class DrupalCommerceFullTest extends AbstractTestCase
{

    public $runner;

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testGeneralFunctions() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'settings_check' => true,
            )
        );
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testUsdPaymentInstant() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'capture_mode' => 'Instant',
            )
        );
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testUsdPaymentDelayed() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'currency'     => 'USD',
                'capture_mode' => 'Delayed',
            )
        );
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testRonPaymentDelayed() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'capture_mode' => 'Delayed',
                'currency'     => 'RON',
            )
        );
    }


    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testEURPaymentInstant() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'capture_mode' => 'Instant',
                'currency'     => 'EUR',
            )
        );
    }

    /**
     * @throws NoSuchElementException
     * @throws TimeOutException
     * @throws UnexpectedTagNameException
     */
    public function testDkkPaymentInstant() {
        $this->runner = new DrupalCommerceRunner($this);
        $this->runner->ready(array(
                'currency'     => 'DKK',
                'capture_mode' => 'Instant'
            )
        );
    }


}
