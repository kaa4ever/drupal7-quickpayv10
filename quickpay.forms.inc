<?php
/**
 * @file
 * Forms for Quickpay.
 */

function _quickpay_configuration_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'quickpay') . '/quickpay.admin.css');

  $form['quickpay_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#description' => t('This is the Merchant ID from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_merchant_id', ''),
    '#required' => TRUE,
  );

  $form['quickpay_agreement_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Agreement ID'),
    '#description' => t('This is the Agreement ID from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_agreement_id', ''),
    '#required' => TRUE,
  );

  $form['quickpay_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API'),
    '#description' => t('This is the API key from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_api_key', ''),
    '#required' => TRUE,
  );

  $form['quickpay_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private'),
    '#description' => t('This is the private key from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_private_key', ''),
    '#required' => TRUE,
  );

  $form['quickpay_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID prefix'),
    '#description' => t('Prefix for order IDs. Order IDs must be uniqe when sent to QuickPay, use this to resolve clashes.'),
    '#default_value' => variable_get('quickpay_order_prefix', ''),
  );

  $languages = array_merge([LANGUAGE_NONE => t('Language of the user')], _quickpay_languages());
  $form['quickpay_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('The language for the credit card form.'),
    '#options' => $languages,
    '#default_value' => variable_get('quickpay_language', LANGUAGE_NONE),
  );

  $form['quickpay_payment_method'] = array(
    '#type' => 'radios',
    '#id' => 'quickpay-method',
    '#title' => t('Accepted payment methods'),
    '#description' => t('Which payment methods to accept. NOTE: Some require special agreements.'),
    '#default_value' => variable_get('quickpay_payment_method', 'creditcard'),
    '#options' => array(
      'creditcard' => t('Creditcard'),
      '3d-creditcard' => t('3D-Secure Creditcard'),
      'selected' => t('Selected payment methods'),
    ),
  );

  $options = array();
  // Add image to the cards where defined.
  foreach (_quickpay_cards() as $key => $card) {
    $options[$key] = empty($card['image']) ? $card['name'] : '<img src="/' . $card['image'] . '" />' . $card['name'];
  }

  $form['quickpay_accepted_cards'] = array(
    '#type' => 'checkboxes',
    '#id' => 'quickpay-cards',
    '#title' => t('Select accepted cards'),
    '#default_value' => variable_get('quickpay_accepted_cards', []),
    '#options' => $options,
    '#states' => array(
      'visible' => array(
        ':input[name="quickpay_payment_method"]' => array('value' => 'selected'),
      ),
    ),
  );

  $form['quickpay_autofee'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autofee'),
    '#description' => t('If set, the fee charged by the acquirer will be calculated and added to the transaction amount.'),
    '#default_value' => variable_get('quickpay_autofee', FALSE),
  );

  $form['quickpay_autocapture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocapture'),
    '#description' => t('If set, the transactions will be automatically captured.'),
    '#default_value' => variable_get('quickpay_autocapture', FALSE),
  );

  return system_settings_form($form);
}


/**
 * Returns an array of languages supported by Quickpay.
 *
 * @return array
 *   Array with key being language codes, and value being names.
 */
function _quickpay_languages() {
  return array(
    'da' => t('Danish'),
    'de' => t('German'),
    'en' => t('English'),
    'fo' => t('Faeroese'),
    'fr' => t('French'),
    'gl' => t('Greenlandish'),
    'it' => t('Italian'),
    'no' => t('Norwegian'),
    'nl' => t('Dutch'),
    'pl' => t('Polish'),
    'se' => t('Swedish'),
  );
}

/**
 * Information about all supported cards.
 *
 * @return array
 *   Array with card name and image.
 */
function _quickpay_cards() {
  $images_path = drupal_get_path('module', 'quickpay') . '/images/';
  return array(
    'dankort' => array(
      'name' => t('Dankort'),
      'image' => $images_path . 'dan.jpg',
    ),
    'edankort' => array(
      'name' => t('eDankort'),
      'image' => $images_path . 'edan.jpg',
    ),
    'visa' => array(
      'name' => t('Visa'),
      'image' => $images_path . 'visa.jpg',
    ),
    'visa-dk' => array(
      'name' => t('Visa, issued in Denmark'),
      'image' => $images_path . 'visa.jpg',
    ),
    '3d-visa' => array(
      'name' => t('Visa, using 3D-Secure'),
      'image' => $images_path . '3d-visa.gif',
    ),
    '3d-visa-dk' => array(
      'name' => t('Visa, issued in Denmark, using 3D-Secure'),
      'image' => $images_path . '3d-visa.gif',
    ),
    'visa-electron' => array(
      'name' => t('Visa Electron'),
      'image' => $images_path . 'visaelectron.jpg',
    ),
    'visa-electron-dk' => array(
      'name' => t('Visa Electron, issued in Denmark'),
      'image' => $images_path . 'visaelectron.jpg',
    ),
    '3d-visa-electron' => array(
      'name' => t('Visa Electron, using 3D-Secure'),
    ),
    '3d-visa-electron-dk' => array(
      'name' => t('Visa Electron, issued in Denmark, using 3D-Secure'),
    ),
    'mastercard' => array(
      'name' => t('Mastercard'),
      'image' => $images_path . 'mastercard.jpg',
    ),
    'mastercard-dk' => array(
      'name' => t('Mastercard, issued in Denmark'),
      'image' => $images_path . 'mastercard.jpg',
    ),
    'mastercard-debet-dk' => array(
      'name' => t('Mastercard debet card, issued in Denmark'),
      'image' => $images_path . 'mastercard.jpg',
    ),
    '3d-mastercard' => array(
      'name' => t('Mastercard, using 3D-Secure'),
    ),
    '3d-mastercard-dk' => array(
      'name' => t('Mastercard, issued in Denmark, using 3D-Secure'),
    ),
    '3d-mastercard-debet-dk' => array(
      'name' => t('Mastercard debet, issued in Denmark, using 3D-Secure'),
    ),
    '3d-maestro' => array(
      'name' => t('Maestro'),
      'image' => $images_path . '3d-maestro.gif',
    ),
    '3d-maestro-dk' => array(
      'name' => t('Maestro, issued in Denmark'),
      'image' => $images_path . '3d-maestro.gif',
    ),
    'jcb' => array(
      'name' => t('JCB'),
      'image' => $images_path . 'jcb.jpg',
    ),
    '3d-jcb' => array(
      'name' => t('JCB, using 3D-Secure'),
      'image' => $images_path . '3d-jcb.gif',
    ),
    'diners' => array(
      'name' => t('Diners'),
      'image' => $images_path . 'diners.jpg',
    ),
    'diners-dk' => array(
      'name' => t('Diners, issued in Denmark'),
      'image' => $images_path . 'diners.jpg',
    ),
    'american-express' => array(
      'name' => t('American Express'),
      'image' => $images_path . 'amexpress.jpg',
    ),
    'american-express-dk' => array(
      'name' => t('American Express, issued in Denmark'),
      'image' => $images_path . 'amexpress.jpg',
    ),
    'danske-dk' => array(
      'name' => t('Danske Netbetaling'),
      'image' => $images_path . 'danskebank.jpg',
    ),
    'nordea-dk' => array(
      'name' => t('Nordea Netbetaling'),
      'image' => $images_path . 'nordea.jpg',
    ),
    'fbg1886' => array(
      'name' => t('Forbrugsforeningen'),
      'image' => $images_path . 'forbrugsforeningen.gif',
    ),
    'ikano' => array(
      'name' => t('Ikano'),
      'image' => $images_path . 'ikano.jpg',
    ),
    'paypal' => array(
      'name' => t('PayPal'),
      'image' => $images_path . 'paypal.jpg',
    ),
    'sofort' => array(
      'name' => t('Sofort'),
      'image' => $images_path . 'sofort.png',
    ),
    'viabill' => array(
      'name' => t('ViaBill'),
      'image' => $images_path . 'viabill.png',
    ),
  );
}
