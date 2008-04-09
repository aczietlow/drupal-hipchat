<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the Bot module.
 */

/**
 * Configures the bot framework; system_settings_form().
 */
function bot_settings() {
  $form = array();

  $form['bot_server'] = array(
    '#default_value' => variable_get('bot_server', 'irc.freenode.net'),
    '#description'   => t('Enter the IRC server the bot will connect to.'),
    '#title'         => t('IRC server'),
    '#type'          => 'textfield',
  );
  $form['bot_server_port'] = array(
    '#default_value' => variable_get('bot_server_port', 6667),
    '#description'   => t('Enter the IRC port of the IRC server. 6667 is the most common configuration.'),
    '#title'         => t('IRC server port'),
    '#type'          => 'textfield',
  );
  $form['bot_nickname'] = array(
    '#default_value' => variable_get('bot_nickname', 'bot_module'),
    '#description'   => t('Enter the nickname the bot will login as.'),
    '#title'         => t('Bot nickname'),
    '#type'          => 'textfield',
  );
  $form['bot_password'] = array(
    '#default_value' => variable_get('bot_password', ''),
    '#description'   => t('(Optional) Enter the password the bot will login to the server with.'),
    '#title'         => t('Bot password'),
    '#type'          => 'textfield',
  );
  $form['bot_channels'] = array(
    '#default_value' => variable_get('bot_channels', '#test'),
    '#description'   => t('Enter a comma-separated list of channels the bot will join. For channels with a key, use "&lt;#channel> &lt;key>".'),
    '#rows'          => 3,
    '#title'         => t('Bot channels'),
    '#type'          => 'textarea',
  );
  $form['bot_debugging'] = array(
    '#default_value' => variable_get('bot_debugging', 0), // spits out a TON of (useful) stuff.
    '#description'   => t('If checked, send Net_SmartIRC\'s SMARTIRC_DEBUG_ALL to the shell.'),
    '#options'       => array('0' => t('No'), '1' => t('Yes')),
    '#title'         => t('Enable debugging'),
    '#type'          => 'checkbox',
  );

  return system_settings_form($form);
}
