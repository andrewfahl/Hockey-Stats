<?php

/*
 * @File
 * Hockey Stats League pages and functions
 * 
 */

/**
 * League menu item landing page content
 */
function hockey_stats_league_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_league_landing');
}

/**
 * League menu item add league page
 */
function hockey_stats_league_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_league_add');
}

/**
 * League menu item edit league page
 */
function hockey_stats_league_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_league_edit');
}

/**
 * League menu item delete league page
 */
function hockey_stats_league_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_league_delete');
}

/**
 * League menu item league landing page
 */
function hockey_stats_league_landing() {
  $header = array(
       'name' => t('Name'),
       'Sort Order' => t('Sort Order'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();

  $application = new Application();
  $leagues = $application->getLeagues();

  foreach ($leagues as $league) {

    $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'leagueEdit(' . $league->getLeagueId() . ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'leagueDelete(' . $league->getLeagueId() . ');'),
    );

    $options[$league->getLeagueId()] = array(
         'name' => t($league->getName()),
         'sequence' => ($league->getSequence()),
         'edit' => array('data' => $edit_button),
         'delete' => array('data' => $delete_button),
    );
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new league'),
       '#href' => 'base/league/add',
  );

  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'leagueTable',
       '#empty' => t('No leagues found'),
  );

  $form['leagueId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'leagueId'),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

/**
 * Content for the add league form
 */
function hockey_stats_league_add($form_state) {

  $form['league'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add League'),
  );

  $form['league']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('League Name'),
       '#size' => 30,
       '#maxlength' => 30,
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['league']['sequence'] = array(
       '#type' => 'textfield',
       '#title' => t('Sort Order'),
       '#size' => 3,
       '#maxlength' => 3,
  );

  $form['league']['leagueId'] = array(
       '#type' => 'hidden',
       '#value' => '0',
  );

  $form['league']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['league']['cancel'] = Utility::getCancelButton();
  
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );
   
  return $form;
}

function hockey_stats_league_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_league_edit') {
    $league = hockey_stats_build_league_from_form($form_state);
  } 
  elseif (isset($form_state['input']['leagueId'])) {
    $league = new League($form_state['input']['leagueId']);
    $league->build();
  } 
  else {
    throw(new Exception("League not found"));
  }

  $form['league'] = array(
       '#type' => 'fieldset',
       '#title' => t('Edit League'),
  );

  $form['league']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('League Name'),
       '#size' => 30,
       '#maxlength' => 30,
       '#value' => $league->getName(),
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['league']['sequence'] = array(
       '#type' => 'textfield',
       '#title' => t('Sort Order'),
       '#size' => 3,
       '#maxlength' => 3,
       '#value' => $league->getSequence(),
  );

  $form['league']['leagueId'] = array(
       '#type' => 'hidden',
       '#value' => $league->getLeagueId(),
  );

  $form['league']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['league']['cancel'] = Utility::getCancelButton();
  
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );
  
  return $form;
}

function hockey_stats_league_add_validate($form, &$form_state) {
  hockey_stats_validate_league($form, $form_state);
}

function hockey_stats_league_edit_validate($form, &$form_state) {
  hockey_stats_validate_league($form, $form_state);
}

function hockey_stats_validate_league($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $league = hockey_stats_build_league_from_form($form_state);
    $application = new Application();

    // league name is required
    if ($league->getName() == "") {
      form_set_error('name', 'League Name is required');
    }

    $dup_league_name = $application->getLeagueByName($league->getName(), $league->getLeagueId());

    // league name must be unique
    if (isset($dup_league_name)) {
      form_set_error('name', 'The League Name "' . $league->getName() . '" is already in use');
    }

    // sequence must be a number
    if (!is_numeric($league->getSequence()) && $league->getSequence() <= 0) {
      form_set_error('sequence', 'Sort Order must be a number greater than zero');
    }
  }
}

function hockey_stats_league_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Submit") {

    $league = hockey_stats_build_league_from_form($form_state);
    $league->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    $league_original = New League($league->getLeagueId());
    $league_original->build();

    if (!isset($league_original)) {
      $league_original = new League();
    }

    if (!$league->equals($league_original)) {
      $league->save();
    }

    drupal_set_message(t('The league has been saved'));
  }
  $form_state['redirect'] = array('base/league', );
}

function hockey_stats_league_add_submit($form, &$form_state) {
  hockey_stats_league_submit($form, $form_state);
}

function hockey_stats_league_edit_submit($form, &$form_state) {
  hockey_stats_league_submit($form, $form_state);
}

function hockey_stats_build_league_from_form(&$form_state) {

  $league_name = trim($form_state['input']['name']);
  $sequence = trim($form_state['input']['sequence']);
  $league_id = $form_state['input']['leagueId'];
  $league = new League($league_id, $sequence, $league_name);

  return $league;
}

function hockey_stats_league_delete($form, &$form_state) {

  if (isset($form_state['input']['leagueId'])) {
    $application = new Application();
    $league = $application->getLeagueById($form_state['input']['leagueId']);
  } 
  else {
    throw(new Exception("League not found"));
  }

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the "' . $league->getName() . '" League? All data will be lost!',
       '#suffix' => '</p>',
  );

  $form['leagueId'] = array(
       '#type' => 'hidden',
       '#value' => $league->getLeagueId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete League',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_league_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete League") {

    $league_id = $form_state['input']['leagueId'];

    if (isset($league_id)) {
      $league = new League($league_id);
      $league->build();
    } 
    else {
      throw(new Exception("League not found"));
    }

    $league->delete();

    drupal_set_message(t('The league has been deleted'));
  }

  $form_state['redirect'] = array('base/league', );
}
