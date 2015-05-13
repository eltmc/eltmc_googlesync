<?php

function drush_eltmc_googlesync_eltmc_committee() {
    $config = _eltmc_googlesync_get_config();
    if (!$config) {
        echo("eltmc_googlesync not configured\n");
        return;
    }

    // Create an array $group_admins whose keys are the group admin(s).
    // For now, we just allow a single (optional) shared group admin.
    $groups_admin = $config['groups_admin_account'];
    $group_admins = array();
    if ($groups_admin) $group_admins[$groups_admin] = true;

    // Create an assoc array mapping committe_role official emails to
    // assoc arrays of users' email addresses (whose keys are
    // important, but the values are ignored)
    $groups = _eltmc_googlesync_create_recipient_map(array_keys($group_admins), $config);
    if (!$groups) {
        echo("eltmc_googlesync could not determine the committee users\n");
        echo("Check the logged errors with 'drush watchdog-show'\n");
        return;
    }

    var_dump($groups);
}


function drush_eltmc_googlesync_eltmc_delta() {
    $delta = _eltmc_googlesync_sync_groups_to_committee(true);

    var_dump($delta);
}


function drush_eltmc_googlesync_eltmc_sync() {
    $delta = _eltmc_googlesync_sync_groups_to_committee();

    var_dump($delta);
}


function drush_eltmc_googlesync_eltmc_google_groups() {
    $config = _eltmc_googlesync_get_config();
    if (!$config) {
        echo("eltmc_googlesync not configured\n");
        return;
    }

    $client = _eltmc_googlesync_get_client($config);
    if (!$client) {
        echo("eltmc_googlesync could not obtain a google client\n");
        echo("Check the logged errors with 'drush watchdog-show'\n");
        return;
    }

    $service = new Google_Service_Directory($client);
    $gapps_groups = _eltmc_googlesync_gapps_groups($service, $config);

    var_dump($gapps_groups);
}


function drush_eltmc_googlesync_eltmc_google_users() {
    $config = _eltmc_googlesync_get_config();
    if (!$config) {
        echo("eltmc_googlesync not configured\n");
        return;
    }

    $client = _eltmc_googlesync_get_client($config);
    if (!$client) {
        echo("eltmc_googlesync could not obtain a google client\n");
        echo("Check the logged errors with 'drush watchdog-show'\n");
        return;
    }

    $service = new Google_Service_Directory($client);

    // Get Google Apps idea of user accounts and aliases
    $gapps_users = _eltmc_googlesync_gapps_users($service, $config);
    var_dump($gapps_users);
}


/**
 * Implements hook_drush_command().
 *
 */
function eltmc_googlesync_drush_command() {

  $items['eltmc-committee'] = array(
      'description' => 'Query Drupal and get the committee users',
      'aliases' => array('egc'),
  );
  $items['eltmc-delta'] = array(
      'description' => 'Query Drupal and Google and get the pending changes',
      'aliases' => array('egd'),
  );
  $items['eltmc-sync'] = array(
      'description' => 'Query Drupal and Google and apply the pending changes',
      'aliases' => array('egs'),
  );
  $items['eltmc-google-groups'] = array(
      'description' => 'Query Google API and get the groups',
      'aliases' => array('egg'),
  );
  $items['eltmc-google-users'] = array(
      'description' => 'Query Google API and get the users',
      'aliases' => array('egu'),
  );

  return $items;
}

