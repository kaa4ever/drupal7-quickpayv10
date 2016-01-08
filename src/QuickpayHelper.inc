<?php
/**
 * @file
 * Helper functions for Quickpay integration.
 */

class QuickpayHelper implements QuickpayHelperInterface {

  /**
   * @inheritdoc
   */
  public function checkParameters(array $parameters) {
    if (!isset($parameters['order_id'])) {
      throw new QuickpayException(t('You must specify an order ID'));
    }
    if (!isset($parameters['currency'])) {
      throw new QuickpayException(t('You must specify a valid currency'));
    }
    if (!isset($parameters['amount'])) {
      throw new QuickpayException(t('You must specify an amount'));
    }
    if (!isset($parameters['continue_url'])) {
      throw new QuickpayException(t('You must specify a continue URL'));
    }
    if (!isset($parameters['cancel_url'])) {
      throw new QuickpayException(t('You must specify a cancel URL'));
    }
  }

  /**
   * @inheritdoc
   */
  public function getChecksumFromRequest() {
    $private_key = variable_get('quickpay_private_key', '');
    $request_body = file_get_contents("php://input");
    return hash_hmac("sha256", $request_body, $private_key);
  }

    /**
   * @inheritdoc
   */
  public function getChecksum(array $data) {
    $api_key = variable_get('quickpay_api_key', '');
    ksort($data);
    $base = implode(" ", $data);
    return hash_hmac("sha256", $base, $api_key);
  }

  /**
   * @inheritdoc
   */
  public function getPaymentMethods() {
    $payment_method = variable_get('quickpay_payment_method', 'creditcard');
    if ($payment_method === 'selected') {
      $accepted_cards = variable_get('quickpay_accepted_cards', []);
      // Aren't supported in cardtypelock.
      unset($accepted_cards['ikano']);
      return implode(',', $accepted_cards);
    }
    return $payment_method;
  }

  /**
   * @inheritdoc
   */
  public function getLanguage() {
    $language_code = variable_get('quickpay_language', LANGUAGE_NONE);
    $language_code = $language_code === LANGUAGE_NONE ? 'en' : $language_code;
    $language_codes = array(
      'da' => 'da',
      'de' => 'de',
      'en' => 'en',
      'fo' => 'fo',
      'fr' => 'fr',
      'kl' => 'gl',
      'it' => 'it',
      'nb' => 'no',
      'nn' => 'no',
      'nl' => 'nl',
      'pl' => 'pl',
      'sv' => 'se',
    );
    return isset($language_codes[$language_code]) ? $language_codes[$language_code] : 'en';
  }

  /**
   * @inheritdoc
   */
  public function wireAmount($amount, array $currency_info) {
    return (function_exists('bcmul') ? bcmul($amount, $currency_info['multiplier']) : $amount * $currency_info['multiplier']);
  }

  /**
   * @inheritdoc
   */
  public function currencyInfo($code) {
    // If the currency is not known, throw an exception.
    if (!array_key_exists($code, $this->getBaseCurrencies())) {
      throw new QuickpayException(t('Unknown currency code %currency', array('%currency' => $code)));
    }
    $base_currencies = $this->getBaseCurrencies();
    return $base_currencies[$code];
  }

  /**
   * A map of currencies know by the module.
   *
   * @inheritdoc
   */
  private function getBaseCurrencies() {
    return array(
      'DKK' => array('code' => 'DKK', 'multiplier' => 100),
      'USD' => array('code' => 'USD', 'multiplier' => 100),
      'EUR' => array('code' => 'EUR', 'multiplier' => 100),
      'GBP' => array('code' => 'GBP', 'multiplier' => 100),
      'SEK' => array('code' => 'SEK', 'multiplier' => 100),
      'NOK' => array('code' => 'NOK', 'multiplier' => 100),
      'ISK' => array('code' => 'ISK', 'multiplier' => 100),
    );
  }
}
