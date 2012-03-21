<?php

/**
 * @file
 * Admin callbacks for the Bot Drupal API module.
 */

/**
 * Configures the Bot Drupal API module.
 */
function bot_drupal_api_settings() {
  $form['bot_drupal_api_function_dumps'] = array(
    '#default_value'  => variable_get('bot_drupal_api_function_dumps', _bot_drupal_api_function_dumps_string()),
    '#description'    => t('Specify the api.module function dump URLs you would like to index.  The order URLs are specified will determine priority if multiple function name matches are found: first found, first served. The syntax takes the form of "&lt;URL>:&lt;LABEL>", where &lt;LABEL> can be optionally used in an IRC request with "check_markup:&lt;LABEL>?". api.module function dump URLs are always in the form of "http://&lt;DRUPAL_URL>/api/function_dump/&lt;API_BRANCH>". The &lt;API_BRANCH> and &lt;LABEL> do not have to match. &lt;LABEL>s should be unique.'),
    '#title'          => t('api.module function dump URLs'),
    '#type'           => 'textarea',
  );
  $form['bot_drupal_api_function_dump_update_frequency'] = array(
    '#default_value'  => variable_get('bot_drupal_api_function_dump_update_frequency', 604800),
    '#options'        => array(86400 => t('Day'), 86400 * 2 => t('2 days'), 86400 * 3 => t('3 days'), 604800 => t('Week'), 604800 * 2 => t('2 weeks'), 604800 * 3 => t('3 weeks')),
    '#title'          => t('Update every'), // we could have use format_interval here, but we didn't like "1 day" and "1 week" as strings.
    '#type'           => 'select',
  );

  return system_settings_form($form);
}
