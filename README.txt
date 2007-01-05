// $Id$

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * IRC API Hooks
 * Cache Warning
 * Design Decisions


INTRODUCTION
------------

Current Maintainer: Morbus Iff <morbus@disobey.com>

Druplicon is an IRC bot that has been servicing #drupal, #drupal-support,
and many other IRC channels since 2005, proving itself an invaluable resource.
Originally a Perl Bot::BasicBot::Pluggable application coded by Morbus Iff,
he always wanted to make the official #drupal bot an actual Drupal module.

This is the fruit of these labors. Whilst the needs of Druplicon are driving
the future and design of the module, this is intended as a generic framework
for IRC bots within Drupal, and usage outside of Druplicon is encouraged.


INSTALLATION
------------

The bot.module is not like other Drupal modules and requires a bit more
effort than normal to get going. Unlike a regular Drupal page load, an
IRC bot has to run forever and, for reasons best explained elsewhere, this
entails running the bot through a shell NOT through web browser access.

1. This module REQUIRES Net_SmartIRC, a PHP class available from PEAR.
   In most cases, you can simply run "pear install Net_SmartIRC".

2. Copy this bot/ directory to your sites/SITENAME/modules directory.

3. Enable the module and configure admin/settings/bot.

4. Inside the bot/ directory is a bot_start.php script which is a wrapper
   around Drupal and the IRC network libraries. To run this script, you'll
   need to open up a shell to that directory and use the following command:

     php bot_start.php --root /path/to/drupal/root --url http://www.example.com

   --root refers to the full path to your Drupal installation directory
   and allows you to execute bot_start.php without moving it to the root
   directory. --url is required (and is equivalent to Drupal's base URL)
   to trick Drupal into thinking that it is being run as through a web
   browser. It sets HTTP_HOST and PHP_SELF, as required by Drupal.

5. Your bot is now started and is trying to connect.


IRC API HOOKS
-------------

The following message types are supported by Net_SmartIRC:

  UNKNOWN     CHANNEL  QUERY     CTCP         NOTICE        WHO
  JOIN        INVITE   ACTION    TOPICCHANGE  NICKCHANGE    KICK
  QUIT        LOGIN    INFO      LIST         NAME          MOTD
  MODECHANGE  PART     ERROR     BANLIST      TOPIC         NONRELEVANT
  WHOIS       WHOWAS   USERMODE  CHANNELMODE  CTCP_REQUEST  CTCP_REPLY

A module may create a function in the form of:

  MODULENAME_irc_msg_MESSAGETYPE

such that a module named "bot_example" could respond or act upon all channel
messages with a function called bot_example_irc_msg_channel(). Passed to
this function is $data, an object reference that contains the message, who
said it, in what channel, and more.

Modules can respond to the user or channel with bot_message($to, $message),
where $to is either a channel name ("#drupal") or user ("Morbus Iff"). Other
IRC actions are demonstrated in the modules shipped with this package. In a
worse case scenario (ie., there's no helper function in bot.module that will
accomplish your desired tasks), you can use "global $irc;" to get the actual
Net_SmartIRC object that represents the IRC connection. Under the most ideal
conditions, you'd contribute back a patch to bot.module that'd let you
accomplish your needs without using the $irc global. Generally speaking,
try not to use the $irc global.

There is another hook available called irc_bot_reply (such that, in our above
example, it'd be bot_example_irc_bot_reply()). This function allows you to
act whenever the bot sends a message. Primarily, this was added to allow us
to log bot responses in bot_log.module. If you use this, be sure NOT to use
bot_message() within your implementation, else you'll cause an infinite loop./

In addition to the actual utility of your module, you also should add a
few lines describing how to use your module. This is done via Drupal's
hook_help(), and the use of two special strings:

 irc:features
   Returns an array of feature names your modules provides.

 irc:features#FEATURE_NAME
   Returns an explanation of a specific feature of your module.

FEATURE_NAME will be lowercased, trimmed of whitespace, and anything not a
letter or number will be turned into an underscore. For an example in code,
take a look at the shipped bot_drupal.module. This information is provided
by the bot under the following conditions:

  <Morbus>     bot_module: help

  <bot_module> Detailed information is available by asking for
               "help <feature>" where <feature> is one of:
               Drupal URLs, dns, karma.

  <Morbus>     bot_module: help Drupal URLs

  <bot_module> Displays the title of drupal.org URLs ...


CACHE WARNING
-------------

Since the IRC bot runs forever, some of Drupal's internal caching mechanisms
(such as variable_get) actually flummox regular operation. We are still working
on workarounds for these (normally desired) features.


DESIGN DECISIONS
----------------

 * We do not enforce command prefixes such as !.

 * We do not enforce direct addressing like "botname: <command>".

Since the entire raw IRC message is passed off to each module, you are more
than welcome to enforce either of the above in your own code. Note that you
WILL have to insure that "botname: <command>" and simply "<command>" both do
as you'd expect - we do not remove "botname:" from the start of messages (and
thus a simple test of "^command$" will fail if the bot is addressed). Most
of our shipped modules cater to these two possibilities.

This is an IRC bot... in PHP. PHP is not especially awesome with regards to
memory management, and it certainly wasn't intended to run a script for any
respectable period of time (like, say, longer than the default 30 seconds).
Likewise, there's no way to uninclude a file, so any change to the loaded
modules (either codewise or enabled/disabled) will require the bot to be
restarted entirely.

Love the limitations, and craziness, of this project.
