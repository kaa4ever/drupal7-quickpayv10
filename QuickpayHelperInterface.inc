<?php
/**
 * @file
 * Interface for the helper class.
 */

interface QuickpayHelperInterface {

  /**
   * Check if the required parameters is set when building the payment form.
   *
   * @param array $parameters
   *   Array of parameters.
   */
  public function checkParameters(array $parameters);

  /**
   * Calculate the md5checksum for the request.
   *
   * http://tech.quickpay.net/payments/hosted/#checksum
   *
   * @param array $data
   *   The data to POST to Quickpay.
   *
   * @return string
   *   The checksum.
   */
  public function getChecksum(array $data);

  /**
   * Return the proper cardtypelock for the accepted cards.
   *
   * @return string
   *   The cards accepted in string format.
   */
  public function getPaymentMethods();

  /**
   * Get the language of the user.
   *
   * @return string
   *   The language code. Defaults to 'en'.
   */
  public function getLanguage();

  /**
   * Get information about an currency.
   *
   * @param string $code
   *   The ISO 4217 currency code.
   *
   * @return mixed
   *   An array with the keys 'code' and 'multiplier'.
   *
   * @throws \Drupal\quickpay\QuickpayException
   *   If the currency code could not be parsed.
   */
  public function currencyInfo($code);

  /**
   * Returns the amount adjusted by the multiplier for the currency.
   *
   * @param int $amount
   *   The amount to wire.
   * @param array $currency_info
   *   An currency_info() array.
   *
   * @return string
   *   The adjusted amount.
   */
  public function wireAmount($amount, array $currency_info);
}
