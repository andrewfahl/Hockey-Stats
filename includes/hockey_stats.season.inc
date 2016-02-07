<?php

/**
 * @file
 * Hockey Stats Module Season Pages and Functions
 */

/**
 * Season menu item landing page
 */
function hockey_stats_season_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_season_landing');
}

/**
 * Season menu item add season page
 */
function hockey_stats_season_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_season_add');
}

/**
 * Season menu item edit season page
 */
function hockey_stats_season_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_season_edit');
}

/**
 * Season menu item delete season page
 */
function hockey_stats_season_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_season_delete');
}

/**
 * Season menu item add teams to season page
 */
function hockey_stats_season_edit_teams_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_season_edit_teams');
}

/**
 * Season menu item season landing page
 */
function hockey_stats_season_landing() {

  $header = array(
       'league' => t('League'),
       'name' => t('Name'),
       'startDateTime' => t('Start Date'),
       'edit' => t('Edit'),
       'editTeams' => t('Add/Remove Teams From Season'),
       'delete' => t('Delete'),
  );

  $options = array();

  $application = new Application();
  $leagues = $application->getLeagues();

  foreach ($leagues as $league) {

    $seasons = $league->getSeasons();

    foreach ($seasons as $season) {

      $edit_button = array(
           '#type' => 'button',
           '#value' => t('Edit'),
           '#attributes' => array('onclick' => 'seasonEdit(' . $season->getSeasonId() . ');'),
      );

      $delete_button = array(
           '#type' => 'button',
           '#value' => t('Delete'),
           '#attributes' => array('onclick' => 'seasonDelete(' . $season->getSeasonId() . ');'),
      );

      $add_teams_button = array(
           '#type' => 'button',
           '#value' => t('Edit Teams'),
           '#attributes' => array('onclick' => 'seasonEditTeams(' . $season->getSeasonId() . ');'),
      );

      $options[$season->getSeasonId()] = array(
           'league' => t($league->getName()),
           'name' => t($season->getName()),
           'startDateTime' => t(Utility::formatDate($season->getStartDateTime())),
           'edit' => array('data' => $edit_button),
           'editTeams' => array('data' => $add_teams_button),
           'delete' => array('data' => $delete_button),
      );
    }
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new season'),
       '#href' => 'base/season/add',
  );

  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'seasonTable',
       '#empty' => t('No seasons found'),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'seasonId'),
  );

  return $form;
}

function hockey_stats_season_add($form_state = NULL) {

  $application = new Application();
  $default_league = $application->getActiveByDefaultLeague();

  if (!isset($default_league)) {

    $form['heading'] = array(
         '#prefix' => '<p>',
         '#markup' => 'You must create a league before you can create a season',
         '#suffix' => '</p>',
    );

    $form['form']['cancel'] = Utility::getCancelButton();
  }
  else {

    $form['form'] = array(
         '#type' => 'fieldset',
         '#title' => t('Add Season'),
    );

    $form['form']['leagueId'] = array(
         '#type' => 'select',
         '#title' => t('League'),
         '#options' => $application->getLeagueList(),
         '#default_value' => $default_league->getLeagueId(),
    );

    $form['form']['name'] = array(
         '#type' => 'textfield',
         '#title' => t('Season Name'),
         '#size' => 30,
         '#maxlength' => 30,
         '#attributes' => array(
           'class' => array(
             'setFocusOnLoad' 
           ),
         ),
    );

    $form['form']['startDateTime'] = array(
         '#type' => 'textfield',
         '#title' => t('Start Date'),
         '#size' => 12,
         '#maxlength' => 12,
         '#attributes' => array('class' => array('datepicker')),
    );

    $form['form']['seasonId'] = array(
         '#type' => 'hidden',
         '#value' => '0',
    );

    $form['form']['submit'] = array(
         '#type' => 'submit',
         '#value' => 'Submit',
    );

    $form['form']['cancel'] = Utility::getCancelButton();

    $form['#after_build'] = array('hockey_stats_add_uidatepicker');

  }
  return $form;
}

function hockey_stats_season_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_season_edit') {
    $season = hockey_stats_build_season_from_form($form_state);
  }
  elseif (isset($form_state['input']['seasonId'])) {
    $season = new Season($form_state['input']['seasonId']);
    $season->build();
  }
  else {
    throw(new Exception("Season not found"));
  }

  $application = new Application();

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Edit Season'),
  );

  $form['form']['leagueId'] = array(
       '#type' => 'select',
       '#title' => t('League'),
       '#options' => $application->getLeagueList(),
       '#default_value' => $season->getLeagueId(),
  );

  $form['form']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('Season Name'),
       '#size' => 30,
       '#maxlength' => 30,
       '#value' => $season->getName(),
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['form']['startDateTime'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Date'),
       '#size' => 12,
       '#maxlength' => 12,
       '#value' => Utility::formatDate($season->getStartDateTime()),
       '#attributes' => array('class' => array('datepicker')),
  );

  $form['form']['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => $season->getSeasonId(),
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  $form['#after_build'] = array('hockey_stats_add_uidatepicker');

  return $form;
}

function hockey_stats_season_add_validate($form, &$form_state) {
  hockey_stats_validate_season($form, $form_state);
}

function hockey_stats_season_edit_validate($form, &$form_state) {
  hockey_stats_validate_season($form, $form_state);
}

function hockey_stats_validate_season($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $season = hockey_stats_build_season_from_form($form_state);
    $league = new League($season->getLeagueId());
    $league->build();

    $dup_season_name = $league->getSeasonByName($season->getName(), $season->getSeasonId());

    // league is required
    if ($season->getLeagueId() <= 0) {
      form_set_error('leagueId', 'The League is required');
    }

    // season name is required
    if ($season->getName() == "") {
      form_set_error('name', 'The Season Name is required');
    }
    // season name already exists for the league
    elseif (isset($dup_season_name)) {
      form_set_error('name', 'The Season Name "' . $season->getName() . '" is already in use');
    }

    // start date is not valid
    if (!Utility::isValidDate($season->getStartDateTime())) {
      form_set_error('startDateTime', 'The Start Date is not valid (DD/MM/YYYY Format)');
    }
  }
}

function hockey_stats_season_delete($form, &$form_state) {

  if (isset($form_state['input']['seasonId'])) {
    $season = new Season($form_state['input']['seasonId']);
    $season->build();
  }
  else {
    throw(new Exception("Season not found"));
  }

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the "' . $season->getName() . '" Season?  All data will be lost!',
       '#suffix' => '</p>',
  );

  $form['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => $season->getSeasonId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Season',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_season_edit_submit($form, &$form_state) {
  hockey_stats_season_add_submit($form, $form_state);
}

function hockey_stats_season_add_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $season = hockey_stats_build_season_from_form($form_state);
    $season->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    // make sure the start date field is formatted correctly 
    $season->setStartDateTime(Utility::DBFormatDate($season->getStartDateTime()));

    $season_original = new Season($season->getSeasonId());
    $season_original->build();

    if (!isset($season_original)) {
      $season_original = new Season();
    }

    if (!$season->equals($season_original)) {
      $season->save();
    }

    drupal_set_message(t('The season has been saved'));
  }
  $form_state['redirect'] = array('base/season', );
}

function hockey_stats_season_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Season") {
    $season_id = $form_state['input']['seasonId'];

    if (isset($season_id)) {
      $season = new Season($season_id);
      $season->build();
    }
    else {
      throw(new Exception("Season not found"));
    }

    $season->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $season->delete();

    drupal_set_message(t('The season has been deleted'));
  }

  $form_state['redirect'] = array('base/season', );
}

function hockey_stats_build_season_from_form(&$form_state) {

  $season_id = $form_state['input']['seasonId'];
  $league_id = $form_state['input']['leagueId'];
  $season_name = trim($form_state['input']['name']);
  $start_date_time = trim($form_state['input']['startDateTime']);

  $season = new Season($season_id, $league_id, $start_date_time, $season_name);

  return $season;
}

function hockey_stats_season_edit_teams($form, &$form_state) {

  if (isset($form_state['input']['seasonId'])) {
    $season = new Season($form_state['input']['seasonId']);
    $season->build();

    $league = new League($season->getLeagueId());
    $league->build();
  }
  else {
    throw new Exception('Season not found');
  }

  $application = new Application();
  $teams = $application->getActiveTeams();

  $checkboxes = array(); // all of the team names
  $ischecked = array(); // if the team is in the season or not

  foreach ($teams as $team) {

    $checkboxes[$team->getTeamId()] = $team->getName();

    if ($season->teamIsInSeasonYN($team->getTeamId())) {
      $ischecked[$team->getTeamId()] = $team->getTeamId();
    }
    else {
      $ischecked[$team->getTeamId()] = '';
    }
  }

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add or remove teams from the ' . $league->getName() . ' ' . $season->getName() . ' season'),
  );

  $form['form']['teams'] = array(
       '#type' => 'checkboxes',
       '#options' => $checkboxes,
       '#default_value' => $ischecked,
  );

  $form['form']['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => $season->getSeasonId(),
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_season_edit_teams_validate($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $team_ids = array_filter($form_state['values']['teams']);

    if (sizeof($team_ids) == 0) {
      form_set_error('', 'You must select at least one team');
    }
  }
}

function hockey_stats_season_edit_teams_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {

    if (isset($form_state['input']['seasonId'])) {
      $season = new Season($form_state['input']['seasonId']);
      $season->build();
      $season->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    }
    else {
      throw new Exception('Season not found');
    }

    $team_ids = array_filter($form_state['values']['teams']);
    $season_teams = hockey_stats_build_season_team_array($season->getSeasonId(), $team_ids);
    $season->saveTeams($season_teams);

    drupal_set_message(t('Teams have been saved'));
  }

  $form_state['redirect'] = array('base/season', );
}

function hockey_stats_build_season_team_array($season_id, $team_id_array) {

  $season_teams = array();

  foreach ($team_id_array as $team_id) {
    $season_team = new SeasonTeam(0, $season_id, $team_id);
    $season_team->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    array_push($season_teams, $season_team);
  }

  return $season_teams;
}

function hockey_stats_add_uidatepicker($form, $form_state) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  return $form;
}
