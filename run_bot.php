<?php
// $Id$

/*
 * @file
 * A simple wrapper around the Drupal bootstrap and Net_SmartIRC.
 * The first half is devoted to fooling Drupal into thinking it's being run
 * through the web, and the second half gets the IRC library hooked into
 * bot.module in a Drupal-friendly hook-like way.
 */

set_time_limit(0); // ignore max_execution_time.
$script_name = array_shift($_SERVER['argv']);

// --root allows us to keep run_bot.php in the bot.module directory
// without moving it to the root of the Drupal install, and --url
// is required to trick Drupal into thinking it's a web request.
// --url should be the same thing as your Drupal $base_url.
while ($param = array_shift($_SERVER['argv'])) {
  switch ($param) {
    case '--root':
      $drupal_root = array_shift($_SERVER['argv']);
      if (is_dir($drupal_root)) { chdir($drupal_root); }
      else { die("ERROR: $drupal_root not found.\n"); }
      break;

    case '--url':
      $drupal_base_url = parse_url(array_shift($_SERVER['argv']));
      $_SERVER['HTTP_HOST'] = $drupal_base_url['host'];
      $_SERVER['PHP_SELF'] = $drupal_base_url['path'].'/'.$script_name;
      $_SERVER['REQUEST_URI'] = $_SERVER['PHP_SELF'];
      $_SERVER['REMOTE_ADDR'] = NULL; // any values here do...
      $_SERVER['REQUEST_METHOD'] = NULL; // ...odd things. uh huh.
      break;
  }
}

// load in our required libraries.
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
require_once('Net/SmartIRC.php');

// initialize the bot based on sane defaults.
global $irc; $bot = new run_bot_wrapper(); $irc = new Net_SmartIRC();
$irc->setDebug( variable_get('bot_debugging', 0) ? SMARTIRC_DEBUG_ALL : SMARTIRC_DEBUG_NONE );
$irc->setAutoReconnect(TRUE);  // reconnect to the server if disconnected.
$irc->setAutoRetry(TRUE);      // retry if a server connection fails.
$irc->setChannelSyncing(TRUE); // keep a list of joined users per channel.
$irc->setUseSockets(TRUE);     // uses real sockets instead of fsock().

// send every message type the library supports to our wrapper class.
$irc->registerActionhandler(SMARTIRC_TYPE_CHANNEL, '^.*$', $bot, 'bot_class_irc_msg_channel');

// connect and begin listening.
$irc->connect( variable_get('bot_server', 'irc.freenode.net'), variable_get('bot_server_port', 6667) );
$irc->login( variable_get('bot_nickname', 'bot_module'), variable_get('bot_nickname', 'bot_module').' :http://drupal.org/project/bot', 8, variable_get('bot_nickname', 'bot_module') );
$irc->join(preg_split('/\s*,\s*/', variable_get('bot_channels', '#test')));
$irc->listen(); // go into the forever loop - no code after this is run.
$irc->disconnect(); // if we stop listening, disconnect properly.

// accept all the message type that the IRC library knows about
// and pass it off to our master bot hooks, which in turn passes
// it off to all the enabled plugin modules via Drupal's hook system.
class run_bot_wrapper {
  function bot_class_irc_msg_channel(&$irc, &$data) { bot_hook_irc_msg_channel($data); }
}

