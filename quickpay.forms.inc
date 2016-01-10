<?php
/**
 * @file
 * Forms for Quickpay.
 */

function quickpay_configuration_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'quickpay') . '/quickpay.admin.css');

  $form['fs_quickpay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quickpay configuration'),
    '#collapsible' => TRUE,
  );

  $form['fs_quickpay']['quickpay_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#description' => t('This is the Merchant ID from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_merchant_id', ''),
    '#required' => TRUE,
  );

  $form['fs_quickpay']['quickpay_agreement_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Agreement ID'),
    '#description' => t('This is the Agreement ID from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_agreement_id', ''),
    '#required' => TRUE,
  );

  $form['fs_quickpay']['quickpay_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API'),
    '#description' => t('This is the API key from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_api_key', ''),
    '#required' => TRUE,
  );

  $form['fs_quickpay']['quickpay_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private'),
    '#description' => t('This is the private key from the Quickpay manager.'),
    '#default_value' => variable_get('quickpay_private_key', ''),
    '#required' => TRUE,
  );

  $form['fs_quickpay']['quickpay_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID prefix'),
    '#description' => t('Prefix for order IDs. Order IDs must be uniqe when sent to QuickPay, use this to resolve clashes.'),
    '#default_value' => variable_get('quickpay_order_prefix', ''),
  );

  $languages = array_merge([LANGUAGE_NONE => t('Language of the user')], _quickpay_languages());
  $form['fs_quickpay']['quickpay_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('The language for the credit card form.'),
    '#options' => $languages,
    '#default_value' => variable_get('quickpay_language', LANGUAGE_NONE),
  );

  $form['fs_quickpay']['quickpay_payment_method'] = array(
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
  foreach (_quickpay_payment_methods() as $key => $card) {
    $options[$key] = empty($card['image']) ? $card['name'] : '<img src="/' . $card['image'] . '" />' . $card['name'];
  }

  $form['fs_quickpay']['quickpay_accepted_cards'] = array(
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

  $form['fs_quickpay']['quickpay_autofee'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autofee'),
    '#description' => t('If set, the fee charged by the acquirer will be calculated and added to the transaction amount.'),
    '#default_value' => variable_get('quickpay_autofee', FALSE),
  );

  $form['fs_quickpay']['quickpay_autocapture'] = array(
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
function _quickpay_payment_methods() {
  $images_path = drupal_get_path('module', 'quickpay') . '/images/';
  return array(
    'american-express' => array(
      'name' => t('American Express'),
      'image' => $images_path . 'amexpress.jpg',
    ),
    'dankort' => array(
      'name' => t('Dankort'),
      'image' => $images_path . 'dan.jpg',
    ),
    'diners' => array(
      'name' => t('Diners'),
      'image' => $images_path . 'diners.jpg',
    ),
    'edankort' => array(
      'name' => t('eDankort'),
      'image' => $images_path . 'edan.jpg',
    ),
    'fbg1886' => array(
      'name' => t('Forbrugsforeningen'),
      'image' => $images_path . 'forbrugsforeningen.gif',
    ),
    'jcb' => array(
      'name' => t('JCB'),
      'image' => $images_path . 'jcb.jpg',
    ),
    'maestro' => array(
      'name' => t('Maestro'),
      'image' => $images_path . '3d-maestro.gif',
    ),
    'mastercard' => array(
      'name' => t('Mastercard'),
      'image' => $images_path . 'mastercard.jpg',
    ),
    'mastercard-debet' => array(
      'name' => t('Mastercard debet card'),
      'image' => $images_path . 'mastercard.jpg',
    ),
    'mobilepay' => array(
      'name' => t('MobilePay Online'),
      'image' => $images_path . 'mobilepay.jpg',
    ),
    'visa' => array(
      'name' => t('Visa'),
      'image' => $images_path . 'visa.jpg',
    ),
    'visa-electron' => array(
      'name' => t('Visa Electron'),
      'image' => $images_path . 'visaelectron.jpg',
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
    'paii' => array(
      'name' => t('Paii'),
      'image' => $images_path . 'viabill.png',
    ),
  );
}
