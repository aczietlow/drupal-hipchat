// $Id$

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


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
   and allows you to execute run_bot.php without moving it to the root
   directory. --url is required (and is equivalent to Drupal's base URL)
   to trick Drupal into thinking that it is being run as through a web
   browser. It sets HTTP_HOST and PHP_SELF, as required by Drupal.

5. Your bot is now started and is trying to connect.

