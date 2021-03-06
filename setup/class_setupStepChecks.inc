<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2003  Cajus Pollmeier
  Copyright (C) 2011-2016  FusionDirectory

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

class setupStepChecks extends setupStep
{
  var $header_image = 'geticon.php?context=categories&icon=checks&size=48';

  static function getAttributesInfo()
  {
    return array(
      'modules' => array(
        'name'      => _('PHP module and extension checks'),
        'template'  => get_template_path("setup_checks.tpl", TRUE, dirname(__FILE__)),
        'attrs'     => array(
          new FakeAttribute('basic_checks')
        )
      ),
      'setup' => array(
        'name'      => _('PHP setup configuration (<a href="?info" target="_blank">show information</a>)'),
        'template'  => get_template_path("setup_checks.tpl", TRUE, dirname(__FILE__)),
        'attrs'     => array(
          new FakeAttribute('config_checks')
        )
      )
    );
  }

  function update_strings()
  {
    $this->s_short_name   = _('Installation check');
    $this->s_title        = _('Installation check');
    $this->s_description  = _('Basic checks for PHP compatibility and extensions');
  }

  /* Execute and display template */
  function execute()
  {
    $this->run_checks();
    return parent::execute();
  }

  /* Execute all checks */
  function run_checks()
  {
    $basic_checks   = array();
    $config_checks  = array();

    /* PHP version check */
    $N = _('Checking PHP version');
    $D = sprintf(_('PHP must be of version %s or above.'), PHP_MIN_VERSION);
    $S = _('Please upgrade to a supported version.');
    $R = version_compare(phpversion(), PHP_MIN_VERSION, '>=');
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    /* Check for LDAP extension */
    $N = msgPool::checkingFor("LDAP");
    $D = _("FusionDirectory requires this module to talk with your LDAP server.");
    $S = msgPool::installPhpModule("LDAP");
    $R = is_callable("ldap_bind");
    $M = TRUE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check for gettext support */
    $N = msgPool::checkingFor("gettext");
    $D = _("FusionDirectory requires this module for an internationalized interface.");
    $S = msgPool::installPhpModule("gettext");
    $R = is_callable("bindtextdomain");
    $M = TRUE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check for curl support */
    $N = msgPool::checkingFor('curl');
    $D = _('FusionDirectory requires this module to communicate with different types of servers and protocols.');
    $S = msgPool::installPhpModule('curl');
    $R = is_callable('curl_init');
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    /* Pseudo-random check */
    $N = _('Checking cryptographically secure pseudo-random integers');
    $D = _('You must use PHP>=7 or have openssl module activated so that FusionDirectory can generate cryptographically secure pseudo-random integers.');
    $S = _('Please upgrade to PHP7 or activate openssl module.');
    $R = (is_callable('random_int') || is_callable('openssl_random_pseudo_bytes'));
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    /* Check for json support */
    $N = msgPool::checkingFor('json');
    $D = _('FusionDirectory requires this module to encode variables for javascript use.');
    $S = msgPool::installPhpModule('json');
    $R = is_callable('json_encode');
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    /* Check for filter support */
    $N = msgPool::checkingFor('filter');
    $D = _('FusionDirectory requires this module to filters a variable with a specified filter.');
    $S = msgPool::installPhpModule('filter');
    $R = is_callable('filter_var');
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    /* Check for iconv */
    $N = msgPool::checkingFor("iconv");
    $D = _("FusionDirectory requires this module for the samba integration.");
    $S = msgPool::installPhpModule("iconv");
    $R = is_callable("iconv");
    $M = TRUE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check for installed mhash module */
    $N = msgPool::checkingFor("hash method");
    $D = _("FusionDirectory requires either 'mhash' or the 'sha1' module to make use of SSHA encryption.");
    $S = msgPool::installPhpModule("mhash/sha1");
    $R = is_callable('mhash') || is_callable('sha1');
    $M = FALSE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check if imap module is available */
    $N = msgPool::checkingFor("IMAP");
    $D = _("FusionDirectory requires this module to talk to an IMAP server.");
    $S = msgPool::installPhpModule("IMAP");
    $R = is_callable("imap_open");
    $M = TRUE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check if mbstring module is available */
    $N = msgPool::checkingFor(_("mbstring"));
    $D = _("FusionDirectory requires this module to handle unicode strings.");
    $S = msgPool::installPhpModule("mbstring");
    $R = is_callable("mb_strlen");
    $M = TRUE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    $N = msgPool::checkingFor(_('imagick'));
    $D = _('FusionDirectory requires this extension to handle images.');
    $S = msgPool::installPhpModule('imagick');
    $R = class_exists('Imagick', FALSE);
    $M = TRUE;
    $basic_checks[] = array('NAME' => $N , 'DESC' => $D , 'RESULT' => $R , 'SOLUTION' => $S , 'MUST' => $M );

    $N = msgPool::checkingFor(_("compression module"));
    $D = _("FusionDirectory requires this extension to handle snapshots.");
    $S = msgPool::installPhpModule("compile with --with-zlib");
    $R = is_callable("gzcompress");
    $M = FALSE;
    $basic_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* PHP Configuration checks */

    /* Register_globals off */
    $N = "register_globals = <b>off</b>";
    $D = _("register_globals is a PHP mechanism to register all global variables to be accessible from scripts without changing the scope. This may be a security risk.");
    $S = _("Search for 'register_globals' in your php.ini and switch it to 'Off'.");
    $R = ini_get("register_globals") == 0;
    $M = FALSE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* session lifetime set to >=86400 seconds ? */
    $N = "session.gc_maxlifetime &gt;= <b>86400</b>";
    $D = _("PHP uses this value for the garbage collector to delete old sessions.")." ".
         _("Setting this value to one day will prevent loosing session and cookies before they really timeout.");
    $S = _("Search for 'session.gc_maxlifetime' in your php.ini and set it to 86400 or higher.");
    $R = ini_get("session.gc_maxlifetime") >= 86400;
    $M = FALSE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Session auto start must be turned off */
    $session_auto_start = ini_get('session.auto_start');
    $N = "session.auto_start = <b>"._("Off")."</b>";
    $D = _("In Order to use FusionDirectory without any trouble, the session.auto_start option in your php.ini should be set to 'Off'.");
    $S = _("Search for 'session.auto_start' in your php.ini and set it to 'Off'.");
    $R = !$session_auto_start;
    $M = TRUE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check if memory limit is set to 128 or > */
    $N = "memory_limit &gt;= <b>128</b>";
    $D = _("FusionDirectory needs at least 128MB of memory. Setting it below this limit may cause errors that are not reproducable! Increase it for larger setups.");
    $S = _("Search for 'memory_limit' in your php.ini and set it to '128M' or higher.");
    $R = ini_get('memory_limit') >= 128;
    $M = TRUE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Implicit Flush disabled can increase performance */
    $N = "implicit_flush = <b>"._("Off")."</b>";
    $D = _("This option influences the PHP output handling. Turn this Option off, to increase performance.");
    $S = _("Search for 'implicit_flush' in your php.ini and set it to 'Off'.");
    $R = !ini_get('implicit_flush');
    $M = FALSE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Check if execution time is set to 30 */
    $N = "max_execution_time &gt;= <b>30</b>";
    $D = _("The Execution time should be at least 30 seconds.");
    $S = _("Search for 'max_execution_time' in your php.ini and set it to '30' or higher.");
    $R = ini_get("max_execution_time") >= 30;
    $M = TRUE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Expose php should be set to off */
    $N = "expose_php = <b>"._("Off")."</b>";
    $D = _("Increase the server security by setting expose_php to 'off'. PHP won't send any information about the server you are running in this case.");
    $S = _("Search for 'expose_php' in your php.ini and set if to 'Off'.");
    $R = !ini_get("expose_php");
    $M = FALSE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    /* Emulating old stuff? */
    $N = "zend.ze1_compatibility_mode = <b>"._("Off")."</b>";
    $D = _("Increase your server performance by setting magic_quotes_gpc to 'off'.");
    $S = _("Search for 'zend.ze1_compatibility_mode' in your php.ini and set it to 'Off'.");
    $R = !ini_get('zend.ze1_compatibility_mode');
    $M = FALSE;
    $config_checks[] = array("NAME" => $N , "DESC" => $D , "RESULT" => $R , "SOLUTION" => $S , "MUST" => $M );

    $this->basic_checks   = $basic_checks;
    $this->config_checks  = $config_checks;
  }

  function save_object()
  {
    parent::save_object();
     /* If everything is fine, set this step to completed
     *  and allow switching to next setup step */
    $failed = FALSE;
    foreach (array('basic_checks','config_checks') as $type) {
      foreach ($this->$type as $obj) {
        if ($obj['MUST'] && !$obj['RESULT']) {
          $failed = TRUE;
          break;
        }
      }
    }
    $this->is_completed = !$failed;
  }
}

?>
