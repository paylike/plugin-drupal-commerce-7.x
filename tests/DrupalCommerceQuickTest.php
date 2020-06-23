<?php

namespace DrupalCommerce;


use Facebook\WebDriver\Exception\NoSuchElementException;
use Facebook\WebDriver\WebDriver;
use Facebook\WebDriver\WebDriverBy;
use Facebook\WebDriver\WebDriverExpectedCondition;
use Lmc\Steward\Test\AbstractTestCase;

/**
 * @group drupalcommerce_quick_test
 */
class DrupalCommerceQuickTest extends AbstractTestCase {

	public $runner;

	/**
	 * @throws NoSuchElementException
	 * @throws \Facebook\WebDriver\Exception\TimeOutException
	 * @throws \Facebook\WebDriver\Exception\UnexpectedTagNameException
	 */
	public function testUsdPaymentBeforeOrderInstant() {
		$this->runner = new DrupalCommerceRunner( $this );
		$this->runner->ready( array(
				'capture_mode'           => 'Instant',
				'checkout_mode'          => 'before_order'
			)
		);
	}
}