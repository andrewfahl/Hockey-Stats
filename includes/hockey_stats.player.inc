<?php
/**
 * @file
 * Hockey Stats Module Player Pages and Functions
 * 
 */

/**
 * Player Main Landing Page
 */
function hockey_stats_player_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_player_landing');
}

function hockey_stats_player_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_player_add');
}

/**
 * Player menu item edit page
 */
function hockey_stats_player_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_player_edit');
}

/**
 * Player menu item delete page
 */
function hockey_stats_player_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_player_delete');
}

/**
 * Player menu item landing page
 */
function hockey_stats_player_landing() {

  $header = array(
       'first_name' => t('First Name'),
       'last_name' => t('Last Name'),
       'active_yn' => t('Active'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();

  $application = new Application();
  $players = $application->getPlayers();

  foreach ($players as $player) {

      $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'playerEdit(' . $player->getPlayerId() . ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'playerDelete(' . $player->getPlayerId() . ');'),
    );
      
    $options[$player->getPlayerId()] = array(
         'first_name' => t($player->getFirstName()),
         'last_name' => t($player->getLastName()),
         'active_yn' => $player->getActiveYN() ? 'active' : '',
         'edit' => array('data' => $edit_button),
         'delete' => array('data' => $delete_button),
    );
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new player'),
       '#href' => 'base/player/add',
  );

  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'playerTable',
       '#empty' => t('No Players found'),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['playerid'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'playerid'),
  );

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

/**
 * Player add page content
*/
function hockey_stats_player_add($form_state = NULL) {
  
  $application = new Application();
  $positions = array(0 => '') + $application->getPlayerPositionList();
  
  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add Player'),
  );

  $form['form']['first_name'] = array(
       '#type' => 'textfield',
       '#title' => t('First Name'),
       '#size' => 50,
       '#maxlength' => 255,
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['form']['last_name'] = array(
       '#type' => 'textfield',
       '#title' => t('Last Name'),
       '#size' => 50,
       '#maxlength' => 255,
  );
  
  $form['form']['player_positionid'] = array(
      '#type' => 'select',
      '#title' => t('Primary Position'),
      '#options' => $positions,
  );

  $form['form']['secondary_player_positionid'] = array(
      '#type' => 'select',
      '#title' => t('Secondary Position'),
      '#options' => $positions,
  );
  
  $form['form']['active_yn'] = array(
       '#type' => 'checkbox',
       '#title' => t('Player Is Active'),
       '#default_value' => TRUE,
  );

  $form['form']['playerid'] = array(
       '#type' => 'hidden',
       '#value' => '0',
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();
  
  return $form;
}


function hockey_stats_player_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_player_edit') {
    $player = hockey_stats_build_player_from_form($form_state);
  }
  elseif (isset($form_state['input']['playerid'])) {
    $player = new Player($form_state['input']['playerid']);
    $player->build();
  }
  else {
    throw(new Exception("Player not found"));
  }
  
  $application = new Application();
  $positions = array( 0 => '') + $application->getPlayerPositionList();
  
  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add Player'),
  );

  $form['form']['first_name'] = array(
       '#type' => 'textfield',
       '#title' => t('First Name'),
       '#size' => 50,
       '#maxlength' => 255,
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
       '#value' => $player->getFirstName(),
  );

  $form['form']['last_name'] = array(
       '#type' => 'textfield',
       '#title' => t('Last Name'),
       '#size' => 50,
       '#maxlength' => 255,
       '#value' => $player->getLastName(),
  );
  
  $form['form']['player_positionid'] = array(
      '#type' => 'select',
      '#title' => t('Primary Position'),
      '#options' => $positions,
      '#default_value' => $player->getPlayerPositionId(),
  );

  $form['form']['secondary_player_positionid'] = array(
      '#type' => 'select',
      '#title' => t('Secondary Position'),
      '#options' => $positions,
      '#default_value' => $player->getSecondaryPlayerPositionId(),
  );
  
  $form['form']['active_yn'] = array(
       '#type' => 'checkbox',
       '#title' => t('Player Is Active'),
       '#default_value' => $player->getActiveYN(),
  );

  $form['form']['playerid'] = array(
       '#type' => 'hidden',
       '#value' => $player->getPlayerId(),
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();
  
  return $form;
  
}

function hockey_stats_player_delete($form, &$form_state) {

  if (isset($form_state['input']['playerid'])) {
    $player = new Player($form_state['input']['playerid']);
    $player->build();
  }
  else {
    throw(new Exception("Player not found"));
  }

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the player "' . $player->getLastName() . ', ' . $player->getFirstName() . '"?  All data will be lost!  Your other option is to make this player inactive.  That will preserve the players history.',
       '#suffix' => '</p>',
  );

  $form['playerid'] = array(
       '#type' => 'hidden',
       '#value' => $player->getPlayerId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Player',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_player_add_validate($form, &$form_state) {
  hockey_stats_validate_player($form, $form_state);
}

function hockey_stats_player_edit_validate($form, &$form_state) {
  hockey_stats_validate_player($form, $form_state);
}

function hockey_stats_validate_player($form, &$form_state) {
 if ($form_state['input']['op'] == "Submit") {
    $player = hockey_stats_build_player_from_form($form_state);
    
    $application = new Application();
    $dup_player = $application->getPlayerByName($player->getFirstName(), $player->getLastName(), $player->getPlayerId());

    // first name
    if ($player->getFirstName() == '') {
      form_set_error('first_name', 'First Name is required');
    }
    // last name
    if ($player->getLastName() == '') {
      form_set_error('last_name', 'Last Name is required');
    }
    
    // secondary position cannot be popuated unless primary position is
    if ($player->getSecondaryPlayerPositionId() != 0 && $player->getPlayerPositionId() == 0) {
      form_set_error('secondary_player_positionid', 'Cannot add a Secondary Position without a Primary Position');
    }
    
    // primary position cannot be the same as secondary position
    if ($player->getPlayerPositionId() == $player->getSecondaryPlayerPositionId() && $player->getPlayerPositionId() != 0) {
      form_set_error('secondary_player_positionid', 'Primary and Secondary Postions should be different');
    }
    // player already exists
    if (isset($dup_player)) {
      form_set_error('first_name', 'The Player name "' . $player->getLastName() . ', ' . $player->getFirstName() . '" is already in use');
    }
  }
}

function hockey_stats_player_edit_submit($form, &$form_state) {
  hockey_stats_player_add_submit($form, $form_state);
}

function hockey_stats_player_add_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $player = hockey_stats_build_player_from_form($form_state);
    $player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    $player_original = new Player($player->getPlayerId());
    $player_original->build();

    if (!isset($player_original)) {
      $player_original = new Player();
    }

    if (!$player->equals($player_original)) {
      $player->save();
    }

    drupal_set_message(t('The Player has been saved'));
  }
  $form_state['redirect'] = array('base/player', );
}


function hockey_stats_player_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Player") {
    $player_id = $form_state['input']['playerid'];

    if (isset($player_id)) {
      $player = new Player($player_id);
      $player->build();
    }
    else {
      throw(new Exception("Player not found"));
    }

    $player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $player->delete();

    drupal_set_message(t('The player has been deleted'));
  }

  $form_state['redirect'] = array('base/player', );
}

function hockey_stats_build_player_from_form(&$form_state) {

  $player_id = $form_state['input']['playerid'];
  $first_name = trim($form_state['input']['first_name']);
  $last_name = trim($form_state['input']['last_name']);
  $player_positionid = trim($form_state['input']['player_positionid']);
  $secondary_player_positionid = trim($form_state['input']['secondary_player_positionid']);
  
  if (!isset($form_state['input']['active_yn'])) {
    $active_yn= 0;
  } 
  else {
    $active_yn = 1;
  }
  
  $player = new Player($player_id, $first_name, $last_name, $player_positionid, $secondary_player_positionid, $active_yn);

  return $player;
}