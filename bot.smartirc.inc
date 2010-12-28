<?php
// $Id$

/**
 * @file
 * Classes and functions that wrap Drupal up for Net_SmartIRC.
 */

/**
 * A Drupal wrapper for the various Net_SmartIRC handlers.
 */
class bot_smartirc_wrapper {

  /**
   * Handle all Net_SmartIRC callbacks.
   *
   * @param &$irc
   *    The Net_SmartIRC $irc object.
   * @param &$data
   *    The Net_SmartIRC data object for this message.
   */
  function invoke(&$irc, &$data = NULL) {
    if (isset($data)) {
      // Net_SmartIRC passes data through in the encoding received from the
      // IRC server. The IRC server, in turn, passes it through as received
      // from the remote client. If the remote client is using a single byte
      // encoding (ISO-8559-1 aka Latin-1), this will result in mangled
      // characters or may raise errors for 8-bit characters. Under Drupal 7
      // and PHP 5.3, this caused an "invalid multibyte sequence" warning
      // from htmlspecialchars() and a PDOException that crashed the bot.
      // (To reproduce: use XChat to connect to a server as latin1, but then
      // send an 8-bit Latin-1 character like a Euro [€]). Since we cannot
      // control the encoding of IRC messages like we can control the encoding
      // of web form submissions, we need to force UTF-8 for all messages.
      if (function_exists('mb_detect_encoding')) {
        $encoding = mb_detect_encoding($data->message, 'UTF-8, ISO-8859-1');
        if ($encoding != 'UTF-8') { // no point of converting it already.
          // we only convert over message related elements and leave the
          // existing parses (nick, ident, host, type, channel, etc.) alone.
          $data->message      = mb_convert_encoding($data->message, 'UTF-8', $encoding);
          $data->rawmessage   = mb_convert_encoding($data->rawmessage, 'UTF-8', $encoding);
          $data->messageex    = explode(' ', $data->message);
          $data->rawmessageex = explode(' ', $data->rawmessage);
          watchdog('bot', 'Converted @encoding to UTF-8: @message.', array('@encoding' => $encoding, '@message' => $data->message), WATCHDOG_INFO);
        }
      }
    }

    // The name of the hook is a substring of the class name.
    $hook = drupal_substr(get_class($this), drupal_strlen('bot_'));

    // Give modules an opportunity to stop message processing.
    foreach (module_implements('irc_access') as $module) {
      $function = "{$module}_irc_access";
      $result = $function($hook, $data);

      // Stop handling this request if FALSE.
      if (isset($result) && $result === FALSE) {
        return;
      }
    }

    // Proceed normally.
    module_invoke_all($hook, $data);
  }
}

// Bot cron sub-classes.
class bot_irc_bot_cron          extends bot_smartirc_wrapper {}
class bot_irc_bot_cron_faster   extends bot_smartirc_wrapper {}
class bot_irc_bot_cron_fastest  extends bot_smartirc_wrapper {}

// IRC message sub-classes.
class bot_irc_msg_unknown       extends bot_smartirc_wrapper {}
class bot_irc_msg_channel       extends bot_smartirc_wrapper {}
class bot_irc_msg_query         extends bot_smartirc_wrapper {}
class bot_irc_msg_ctcp          extends bot_smartirc_wrapper {}
class bot_irc_msg_notice        extends bot_smartirc_wrapper {}
class bot_irc_msg_who           extends bot_smartirc_wrapper {}
class bot_irc_msg_join          extends bot_smartirc_wrapper {}
class bot_irc_msg_invite        extends bot_smartirc_wrapper {}
class bot_irc_msg_action        extends bot_smartirc_wrapper {}
class bot_irc_msg_topicchange   extends bot_smartirc_wrapper {}
class bot_irc_msg_nickchange    extends bot_smartirc_wrapper {}
class bot_irc_msg_kick          extends bot_smartirc_wrapper {}
class bot_irc_msg_quit          extends bot_smartirc_wrapper {}
class bot_irc_msg_login         extends bot_smartirc_wrapper {}
class bot_irc_msg_info          extends bot_smartirc_wrapper {}
class bot_irc_msg_list          extends bot_smartirc_wrapper {}
class bot_irc_msg_name          extends bot_smartirc_wrapper {}
class bot_irc_msg_motd          extends bot_smartirc_wrapper {}
class bot_irc_msg_modechange    extends bot_smartirc_wrapper {}
class bot_irc_msg_part          extends bot_smartirc_wrapper {}
class bot_irc_msg_error         extends bot_smartirc_wrapper {}
class bot_irc_msg_banlist       extends bot_smartirc_wrapper {}
class bot_irc_msg_topic         extends bot_smartirc_wrapper {}
class bot_irc_msg_nonrelevant   extends bot_smartirc_wrapper {}
class bot_irc_msg_whois         extends bot_smartirc_wrapper {}
class bot_irc_msg_whowas        extends bot_smartirc_wrapper {}
class bot_irc_msg_usermode      extends bot_smartirc_wrapper {}
class bot_irc_msg_channelmode   extends bot_smartirc_wrapper {}
class bot_irc_msg_ctcp_request  extends bot_smartirc_wrapper {}
class bot_irc_msg_ctcp_reply    extends bot_smartirc_wrapper {}
