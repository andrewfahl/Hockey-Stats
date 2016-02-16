<?php

/**
 * @file
 * Hockey Stats Module Game Pages and Functions
 * 
 */

/**
 * Game Main Landing Page
 */
function hockey_stats_game_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_landing');
}

function hockey_stats_game_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_add');
}

/**
 * Game menu item edit page
 */
function hockey_stats_game_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_edit');
}

/**
 * Game menu item delete page
 */
function hockey_stats_game_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_delete');
}

function hockey_stats_game_goals_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_goals');
}

function hockey_stats_game_penalties_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_landing');
}

function hockey_stats_game_penalties_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_add');
}

function hockey_stats_game_penalties_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_edit');
}

function hockey_stats_game_penalties_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_delete');
}

function hockey_stats_game_attendance_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_attendance');
}

/**
 * Game menu item landing page
 */
function hockey_stats_game_landing($form, &$form_state) {
  $options = array();
  $options[0] = '';

  $application = new Application();
  $leagues = $application->getLeagues();
  $default_league = $application->getActiveByDefaultLeague();
  
  foreach ($leagues as $league) {
    $options[$league->getLeagueId()] = $league->getName();
  }

  $leagueid_cookie = Utility::getCookieValue(Utility::COOKIE_LEAGUEID);
  $seasonid_cookie = Utility::getCookieValue(Utility::COOKIE_SEASONID);

  $leagueid = 0;
  $seasonid = 0;
  
  if (isset($form_state['input']['leagueid'])) {
    $leagueid = $form_state['input']['leagueid'];
  }
  elseif (isset($leagueid_cookie)) {
    $leagueid = $leagueid_cookie;
  }
  elseif (isset($default_league)) {
    $leagueid = $default_league->getLeagueId();
    $season = $default_league->getActiveByDefaultSeason();
    if(isset($season)) {
      $seasonid = $season->getSeasonId();
    }    
  }

  if (isset($form_state['input']['seasonid'])) {
    $seasonid = $form_state['input']['seasonid'];
  }
  elseif (isset($seasonid_cookie)) {
    $seasonid = $seasonid_cookie;
  }

  if ($leagueid > 0) {
    $league = $application->getLeagueById($leagueid);
    
    if (!$league->containsSeason($seasonid)) {
      $seasonid = $league->getActiveByDefaultSeasonId();
    }
  }
  
  if ($leagueid != 0) {
    Utility::setCookieValue(Utility::COOKIE_LEAGUEID, $leagueid);
  }
  
  if ($seasonid != 0) {
    Utility::setCookieValue(Utility::COOKIE_SEASONID, $seasonid);
  }

  // select league
  $form['leagueid'] = array(
       '#type' => 'select',
       '#title' => t('Select League'),
       '#options' => $options,
       '#default_value' => $leagueid,
       '#attributes' => array('onchange' => 'setCookieLeagueId(jQuery(this).val()); document.getElementById("hockey-stats-game-landing").submit()',
            'id' => 'leagueid'),
  );

  // select season
  if ($leagueid > 0) {
    $league = new League($leagueid);
    $league->build();

    $season_options = array(0 => '') + $league->getSeasonList();

    $form['seasonid'] = array(
         '#type' => 'select',
         '#title' => t('Select Season'),
         '#options' => $season_options,
         '#attributes' => array('onchange' => 'setCookieSeasonId(jQuery(this).val()); document.getElementById("hockey-stats-game-landing").submit()',
              'id' => 'seasonid'),
         '#value' => $seasonid,
    );
  }

  // build grid
  if ($seasonid > 0) {

    // make sure the season is in the league else hide grid
    if ($league->containsSeason($seasonid)) {
      $options = array();

      $season = new Season($seasonid);
      $season->build();

      $header = array(
           'game' => t('Teams'),
           'startDateTime' => t('Date'),
           'edit' => t('Edit'),
           'goals' => t('Goals'),
           'penalties' => t('Penalties'),
           'attendance' => t('Attendance'),
           'delete' => t('Delete'),
      );

      $games = $season->getGames();

      foreach ($games as $game) {

        $edit_button = array(
             '#type' => 'button',
             '#value' => t('Edit'),
             '#attributes' => array('onclick' => 'gameEdit(' . $game->getGameId() . ');'),
        );

        $goal_button = array(
             '#type' => 'button',
             '#value' => t('Goals'),
             '#attributes' => array('onclick' => 'gameGoals(' . $game->getGameId() . ');'),
        );

        $penalties_button = array(
             '#type' => 'button',
             '#value' => t('Penalties'),
             '#attributes' => array('onclick' => 'gamePenalties(' . $game->getGameId() . ');'),
        );

        $attendance_button = array(
             '#type' => 'button',
             '#value' => t('Attendance'),
             '#attributes' => array('onclick' => 'gameAttendance(' . $game->getGameId() . ');'),
        );

        $delete_button = array(
             '#type' => 'button',
             '#value' => t('Delete'),
             '#attributes' => array('onclick' => 'gameDelete(' . $game->getGameId() . ');'),
        );

        $options[$game->getGameId()] = array(
             'game' => $game->getVisitingTeamName() . ' vs ' . $game->getHomeTeamName(),
             'startDateTime' => Utility::formatDateTime($game->getStartDateTime()),
             'edit' => array('data' => $edit_button),
             'goals' => array('data' => $goal_button),
             'penalties' => array('data' => $penalties_button),
             'attendance' => array('data' => $attendance_button),
             'delete' => array('data' => $delete_button),
        );
      }

      $form['addLink'] = array(
           '#type' => 'link',
           '#title' => t('Add new game'),
           '#href' => 'base/game/add',
           '#attributes' => array('class' => 'setcookievalues'),
      );

      $form['table'] = array(
           '#theme' => 'table',
           '#header' => $header,
           '#rows' => $options,
           '#id' => 'gameTable',
           '#empty' => t('No games found'),
      );
    }
  }

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'gameId'),
  );
  return $form;
}

function hockey_stats_game_add($form_state = NULL) {

  $seasonid = Utility::getCookieValue(Utility::COOKIE_SEASONID);

  if (!isset($seasonid)) {
    throw new Exception('Season Not Found');
  }

  $season = new Season($seasonid);
  $season->build();

  $application = new Application();

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Game Detail'),
  );

  $form['form']['visiting_teamid'] = array(
       '#type' => 'select',
       '#title' => t('Visiting Team'),
       '#options' => $season->getTeamList(),
  );

  $form['form']['home_teamid'] = array(
       '#type' => 'select',
       '#title' => t('Home Team'),
       '#options' => $season->getTeamList(),
  );

  $form['form']['locationid'] = array(
       '#type' => 'select',
       '#title' => t('Location'),
       '#options' => $application->getLocationList(),
  );

  $form['form']['startDate'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Date'),
       '#size' => 12,
       '#maxlength' => 12,
       '#attributes' => array('class' => array('datepicker')),
  );

  $form['form']['startTime'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Time (24 hour time format)'),
       '#size' => 12,
       '#maxlength' => 12,
       '#attributes' => array('class' => array('timepicker')),
  );

  $form['form']['playoff_game_yn'] = array(
       '#type' => 'checkbox',
       '#title' => t('Playoff Game'),
  );

  $form['form']['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => $season->getSeasonId(),
  );

  $form['form']['gameId'] = array(
       '#type' => 'hidden',
       '#value' => 0,
  );

  $form['form']['startTime'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Time (24 hour time format)'),
       '#size' => 12,
       '#maxlength' => 12,
       '#attributes' => array('class' => array('timepicker')),
  );

  $form['box_score'] = array(
       '#type' => 'fieldset',
       '#title' => t('Box Score'),
  );

  $form['box_score']['visiting_team_first_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<table><thead><tr><th>Team</th><th>1</th><th>2</th><th>3</th><th>OT</th><th>Shots</th></tr></thead><tbody><tr><td>Visiting Team</td><td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['visiting_team_second_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['visiting_team_third_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['visiting_team_over_time_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['visiting_team_shots'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td></tr>',
  );
  // home team
  $form['box_score']['home_team_first_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<tr><td>Home Team</td><td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['home_team_second_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['home_team_third_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['home_team_over_time_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
  );
  $form['box_score']['home_team_shots'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td></tr></tbody></table>',
  );

  $form['box_score']['comment'] = array(
       '#type' => 'textarea',
       '#title' => t('Game Comment'),
       '#maxlength' => 2000,
  );

  $form['#after_build'] = array('hockey_stats_add_uidatepicker');

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_game_edit') {
    $game = hockey_stats_build_game_from_form($form_state);
  }
  elseif (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else {
    throw(new Exception("Game not found"));
  }

  $seasonid = Utility::getCookieValue(Utility::COOKIE_SEASONID);

  if (!isset($seasonid)) {
    throw new Exception('Season Not Found');
  }

  $season = new Season($seasonid);
  $season->build();

  $application = new Application();

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Edit Game'),
  );

  $form['form']['visiting_teamid'] = array(
       '#type' => 'select',
       '#title' => t('Visiting Team'),
       '#options' => $season->getTeamList(),
       '#default_value' => $game->getVisitingTeamId(),
  );

  $form['form']['home_teamid'] = array(
       '#type' => 'select',
       '#title' => t('Home Team'),
       '#options' => $season->getTeamList(),
       '#default_value' => $game->getHomeTeamId(),
  );

  $form['form']['locationid'] = array(
       '#type' => 'select',
       '#title' => t('Location'),
       '#options' => $application->getLocationList(),
       '#default_value' => $game->getLocationId(),
  );

  $form['form']['startDate'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Date'),
       '#size' => 12,
       '#maxlength' => 12,
       '#attributes' => array('class' => array('datepicker')),
       '#value' => Utility::formatDate($game->getStartDateTime()),
  );

  $form['form']['startTime'] = array(
       '#type' => 'textfield',
       '#title' => t('Start Time (24 hour time format)'),
       '#size' => 12,
       '#maxlength' => 12,
       '#attributes' => array('class' => array('timepicker')),
       '#value' => Utility::format24Time($game->getStartDateTime()),
  );

  $form['form']['playoff_game_yn'] = array(
       '#type' => 'checkbox',
       '#title' => t('Playoff Game'),
       '#default_value' => $game->getPlayoffGameYN(),
  );

  $form['form']['seasonId'] = array(
       '#type' => 'hidden',
       '#value' => $season->getSeasonId(),
  );

  $form['form']['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );


  $form['box_score'] = array(
       '#type' => 'fieldset',
       '#title' => t('Box Score'),
  );

  $form['box_score']['visiting_team_first_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<table><thead><tr><th>Team</th><th>1</th><th>2</th><th>3</th><th>OT</th><th>Shots</th></tr></thead><tbody><tr><td>Visiting Team</td><td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getVisitingTeamFirstPeriodGoals(),
  );
  $form['box_score']['visiting_team_second_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getVisitingTeamSecondPeriodGoals(),
  );
  $form['box_score']['visiting_team_third_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getVisitingTeamThirdPeriodGoals(),
  );
  $form['box_score']['visiting_team_over_time_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getVisitingTeamOverTimeGoals(),
  );
  $form['box_score']['visiting_team_shots'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td></tr>',
       '#default_value' => $game->getVisitingTeamShots(),
  );
  // home team
  $form['box_score']['home_team_first_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<tr><td>Home Team</td><td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getHomeTeamFirstPeriodGoals(),
  );
  $form['box_score']['home_team_second_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getHomeTeamSecondPeriodGoals(),
  );
  $form['box_score']['home_team_third_period_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getHomeTeamThirdPeriodGoals(),
  );
  $form['box_score']['home_team_over_time_goals'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td>',
       '#default_value' => $game->getHomeTeamOverTimeGoals(),
  );
  $form['box_score']['home_team_shots'] = array(
       '#type' => 'textfield',
       '#size' => 2,
       '#maxlength' => 2,
       #this is messy, I know but I couldnt find a nice way to do this
       '#prefix' => '<td>',
       '#suffix' => '</td></tr></tbody></table>',
       '#default_value' => $game->getHomeTeamShots(),
  );

  $form['box_score']['comment'] = array(
       '#type' => 'textarea',
       '#title' => t('Game Comment'),
       '#maxlength' => 2000,
       '#default_value' => $game->getComment(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['cancel'] = Utility::getCancelButton();

  $form['#after_build'] = array('hockey_stats_add_uidatepicker');

  return $form;
}

function hockey_stats_game_delete($form, &$form_state) {

  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else {
    throw(new Exception("Game not found"));
  }

  $game_details = $game->getVisitingTeamName() . " vs " . $game->getHomeTeamName();
  $game_details .= " on " . Utility::formatDate($game->getStartDateTime());

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the game "' . $game_details . '"?  All data will be lost!',
       '#suffix' => '</p>',
  );

  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Game',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_add_validate($form, &$form_state) {
  hockey_stats_validate_game($form, $form_state);
}

function hockey_stats_game_edit_validate($form, &$form_state) {
  hockey_stats_validate_game($form, $form_state);
}

function hockey_stats_validate_game($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {

    $game = hockey_stats_build_game_from_form($form_state);
    $start_date = $form_state['input']['startDate'];
    $start_time = $form_state['input']['startTime'];

    // visiting team required
    if ($game->getVisitingTeamId() == 0) {
      form_set_error('visiting_teamid', 'Visiting Team is required');
    }

    // home team required
    if ($game->getHomeTeamId() == 0) {
      form_set_error('home_teamid', 'Home Team is required');
    }

    // location required
    if ($game->getLocationId() == 0) {
      form_set_error('locationid', 'Location is required');
    }

    // start date required
    if ($start_date == NULL) {
      form_set_error('startDate', 'Start Date is required');
    }
    // date is not valid
    elseif (!Utility::isValidDate($start_date)) {
      form_set_error('startDate', 'Start Date is not valid');
    }
    // start time is required
    elseif ($start_time == NULL) {
      form_set_error('startTime', 'Start Time is required');
    }
    // time must be a valid
    elseif (!Utility::isValidTime($start_time)) {
      form_set_error('startTime', 'Start Time is not valid (24 hr format 10:30 PM = 22:30');
    }

    // home and away team cannot be the same
    if ($game->getHomeTeamId() == $game->getVisitingTeamId()) {
      form_set_error('home_teamid', 'Visiting Team and Home Team cannot be the same');
    }

    #home_team_first_period_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getHomeTeamFirstPeriodGoals())) {
      form_set_error('home_team_first_period_goals', 'Home team first period goals must be a positive number');
    }

    #visiting_team_first_period_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getVisitingTeamFirstPeriodGoals())) {
      form_set_error('visiting_team_first_period_goals', 'Visiting team first period goals must be a positive number');
    }

    #home_team_second_period_goals 
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getHomeTeamSecondPeriodGoals())) {
      form_set_error('home_team_second_period_goals', 'Home team second period goals must be a positive number');
    }

    #visiting_team_second_period_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getVisitingTeamSecondPeriodGoals())) {
      form_set_error('visiting_team_second_period_goals', 'Visiting team second period goals must be a positive number');
    }

    #home_team_third_period_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getHomeTeamThirdPeriodGoals())) {
      form_set_error('home_team_third_period_goals', 'Home team third period goals must be a positive number');
    }

    #visiting_team_third_period_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getVisitingTeamThirdPeriodGoals())) {
      form_set_error('visiting_team_third_period_goals', 'Visiting team third period goals must be a positive number');
    }

    #home_team_over_time_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getHomeTeamOverTimeGoals())) {
      form_set_error('home_team_over_time_goals', 'Home team over time goals must be a positive number');
    }

    #visiting_team_over_time_goals
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getVisitingTeamOverTimeGoals())) {
      form_set_error('visiting_team_over_time_goals', 'Visiting team over time goals must be a positive number');
    }

    #home_team_shots
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getHomeTeamShots())) {
      form_set_error('home_team_shots', 'Home team shots must be a positive number');
    }

    #visiting_team_shots
    if (!Utility::isPostiveNumberZeroOrEmpty($game->getVisitingTeamShots())) {
      form_set_error('visiting_team_shots', 'Visiting team shots must be a positive number');
    }
  }
}

function hockey_stats_game_edit_submit($form, &$form_state) {
  hockey_stats_game_add_submit($form, $form_state);
}

function hockey_stats_game_add_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {

    $game = hockey_stats_build_game_from_form($form_state);
    $game->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    $game->setStartDateTime(Utility::DBFormatDate($game->getStartDateTime()));

    $game_original = new Game($game->getGameId());
    $game_original->build();

    if (!isset($game_original)) {
      $game_original = new Game();
    }

    if (!$game->equals($game_original)) {
      $game->save();
    }

    drupal_set_message(t('The game has been saved'));
  }

  $form_state['redirect'] = array('base/game', );
}

function hockey_stats_game_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Game") {
    $game_id = $form_state['input']['gameId'];

    if (isset($game_id)) {
      $game = new Game($game_id);
      $game->build();
    }
    else {
      throw(new Exception("Game not found"));
    }

    $game->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $game->delete();

    drupal_set_message(t('The Game has been deleted'));
  }

  $form_state['redirect'] = array('base/game', );
}

function hockey_stats_build_game_from_form(&$form_state) {

  $gameid = $form_state['input']['gameId'];
  $visiting_teamid = $form_state['input']['visiting_teamid'];
  $home_teamid = $form_state['input']['home_teamid'];
  $locationid = $form_state['input']['locationid'];
  $seasonid = $form_state['input']['seasonId'];
  $start_datetime = trim($form_state['input']['startDate']) . trim($form_state['input']['startTime']);
  $home_team_first_period_goals = trim($form_state['input']['home_team_first_period_goals']);
  $visiting_team_first_period_goals = trim($form_state['input']['visiting_team_first_period_goals']);
  $home_team_second_period_goals = trim($form_state['input']['home_team_second_period_goals']);
  $visiting_team_second_period_goals = trim($form_state['input']['visiting_team_second_period_goals']);
  $home_team_third_period_goals = trim($form_state['input']['home_team_third_period_goals']);
  $visiting_team_third_period_goals = trim($form_state['input']['visiting_team_third_period_goals']);
  $home_team_over_time_goals = trim($form_state['input']['home_team_over_time_goals']);
  $visiting_team_over_time_goals = trim($form_state['input']['visiting_team_over_time_goals']);
  $home_team_shots = trim($form_state['input']['home_team_shots']);
  $visiting_team_shots = trim($form_state['input']['visiting_team_shots']);
  $comment = trim($form_state['input']['comment']);

  if (!isset($form_state['input']['playoff_game_yn'])) {
    $playoff_game_yn = 0;
  }
  else {
    $playoff_game_yn = 1;
  }

  $game = new Game($gameid, $home_teamid, $visiting_teamid, $locationid, $seasonid, $start_datetime, $playoff_game_yn);

  $game->setBoxScore($home_team_first_period_goals, $visiting_team_first_period_goals, $home_team_second_period_goals, $visiting_team_second_period_goals, $home_team_third_period_goals, $visiting_team_third_period_goals, $home_team_over_time_goals, $visiting_team_over_time_goals, $home_team_shots, $visiting_team_shots, $comment);

  return $game;
}

function hockey_stats_add_uidatepicker($form, $form_state) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  return $form;
}

function hockey_stats_game_attendance($form, &$form_state) {

  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }

  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }

  // get the teams
  $home_season_team = new SeasonTeam(NULL, $game->getSeasonId(), $game->getHomeTeamId());
  $home_season_team->build();

  $visiting_season_team = new SeasonTeam(NULL, $game->getSeasonId(), $game->getVisitingTeamId());
  $visiting_season_team->build();

  $home_team_roster = $home_season_team->getSeasonTeamPlayers();
  $visiting_team_roster = $visiting_season_team->getSeasonTeamPlayers();

  $home_team_playerids = $game->getHomeTeamPlayerIds();
  $visiting_team_playerids = $game->getVisitingTeamPlayerIds();

  $default_visiting_goalie = $visiting_season_team->getDefaultGoalie();
  
  // visiting team roster
  $form['visiting_team'] = array(
       '#type' => 'fieldset',
       '#title' => t($game->getVisitingTeamName()),
  );

  $in_game_checkboxes = array();
  $in_game_ischecked = array();

  foreach ($visiting_team_roster as $player) {
    $in_game_checkboxes[$player->getPlayerId()] = $player->getFullName();

    if (in_array($player->getPlayerId(), $visiting_team_playerids)) {
      $in_game_ischecked[$player->getPlayerId()] = $player->getPlayerId();
    }
  }

  $visiting_goalie = $game->getVisitingTeamGoalie();
  
  // if null just pass an empty object
  if (!isset($visiting_goalie)) {
    $visiting_goalie = $visiting_season_team->getDefaultGoalie();
  }
  
  // if null just pass an empty object
  if (!isset($visiting_goalie)) {
    $visiting_goalie = new GamePlayer(0, 0, 0);
  }

  $form['visiting_team']['visiting_players'] = array(
       '#type' => 'checkboxes',
       '#options' => $in_game_checkboxes,
       '#default_value' => $in_game_ischecked,
  );

  $form['visiting_team']['visiting_goalie_playerid'] = array(
       '#type' => 'select',
       '#title' => t('Goalie'),
       '#options' => array(0 => '') + $in_game_checkboxes,
       '#default_value' => $visiting_goalie->getPlayerID(),
  );

  // home team roster
  $form['home_team'] = array(
       '#type' => 'fieldset',
       '#title' => t($game->getHomeTeamName()),
  );

  $in_game_checkboxes = array();
  $in_game_ischecked = array();

  foreach ($home_team_roster as $player) {
    $in_game_checkboxes[$player->getPlayerId()] = $player->getFullName();

    if (in_array($player->getPlayerId(), $home_team_playerids)) {
      $in_game_ischecked[$player->getPlayerId()] = $player->getPlayerId();
    }
  }

  $home_goalie = $game->getHomeTeamGoalie();
  
    // if null just pass an empty object
  if (!isset($home_goalie)) {
    $home_goalie = $visiting_season_team->getDefaultGoalie();
  }
  
  // if null just pass an empty object
  if (!isset($home_goalie)) {
    $home_goalie = new GamePlayer(0, 0, 0);
  }

  $form['home_team']['home_players'] = array(
       '#type' => 'checkboxes',
       '#options' => $in_game_checkboxes,
       '#default_value' => $in_game_ischecked,
  );

  $form['home_team']['home_goalie_playerid'] = array(
       '#type' => 'select',
       '#title' => t('Goalie'),
       '#options' => array(0 => '') + $in_game_checkboxes,
       '#default_value' => $home_goalie->getPlayerID(),
  );

  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  // save cancel
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_attendance_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {

    $game = new Game($form_state['input']['gameId']);
    $game->build();

    // players that participated before
    $previous_home_team_playerids = $game->getHomeTeamPlayerIds();
    $previous_visiting_team_playerids = $game->getVisitingTeamPlayerIds();

    // players that participated in the game
    $visiting_team_playerids = $form_state['input']['visiting_players'];
    $home_team_playerids = $form_state['input']['home_players'];

    // remove nulls and 0's from the lists of players
    if(isset($visiting_team_playerids)) {
      Utility::removeValues($visiting_team_playerids, [0, NULL]);
    }
    
    if(isset($home_team_playerids)) { 
      Utility::removeValues($home_team_playerids, [0, NULL]);
    }

    // set the goalie player ids
    $home_goalie_playerid = $form_state['input']['home_goalie_playerid'];
    $visiting_goalie_playerid = $form_state['input']['visiting_goalie_playerid'];

    // get the previous goalies
    $previous_home_goalie = $game->getHomeTeamGoalie();
    $previous_home_goalie_playerid = 0;
    $previous_visiting_goalie = $game->getVisitingTeamGoalie();
    $previous_visiting_goalie_playerid = 0;

    if (isset($previous_home_goalie)) {
      $previous_home_goalie_playerid = $previous_home_goalie->getPlayerId();
    }

    if (isset($previous_visiting_goalie)) {
      $previous_visiting_goalie_playerid = $previous_visiting_goalie->getPlayerId();
    }

    // set the home attendance if there are players
    if(isset($home_team_playerids)) {
      hockey_stats_game_set_attendance($game->getGameId(), $game->getHomeTeamId(), $home_team_playerids, $previous_home_team_playerids, $home_goalie_playerid, $previous_home_goalie_playerid);
    }
    
    // set the visiting team attendance if there are players
    if(isset($visiting_team_playerids)) {
      hockey_stats_game_set_attendance($game->getGameId(), $game->getVisitingTeamId(), $visiting_team_playerids, $previous_visiting_team_playerids, $visiting_goalie_playerid, $previous_visiting_goalie_playerid);
    }
    
    drupal_set_message(t('Player Attendance has been saved'));
  }

  $form_state['redirect'] = array('base/game', );
}

function hockey_stats_game_set_attendance($gameid, $teamid, $team_playerids, $previous_team_playerids, $goalie_playerid = 0, $previous_goalie_playerid = 0) {

  $add_players = array();
  $remove_players = array();
  $update_players = array();

  $add_team_players = array_diff($team_playerids, $previous_team_playerids);

  foreach ($add_team_players as $playerid) {
    $goalie_yn = $goalie_playerid == $playerid ? 1 : 0;
    $game_player = new GamePlayer($gameid, $teamid, $playerid, $goalie_yn);
    $add_players[$game_player->getPlayerId()] = $game_player;
  }

  // if the home goalie isnt checked add him/her
  if (!in_array($goalie_playerid, $team_playerids) && $goalie_playerid != 0) {
    // add the goalie to the list of players that should be added
    $add_team_players[$goalie_playerid] = $goalie_playerid;

    $goalie_yn = 1;
    $game_player = new GamePlayer($gameid, $teamid, $goalie_playerid, $goalie_yn);
    $add_players[$game_player->getPlayerId()] = $game_player;
  }

  $remove_team_players = array_diff($previous_team_playerids, $team_playerids);

  foreach ($remove_team_players as $playerid) {
    $game_player = new GamePlayer($gameid, $teamid, $playerid);
    $remove_players[$game_player->getPlayerId()] = $game_player;
  }

  // if there is a goalie change
  if ($previous_goalie_playerid != $goalie_playerid) {
    // if the goalie record has not been added it needs to be updated
    if ($goalie_playerid != 0 && !in_array($goalie_playerid, $add_team_players)) {
      $game_player = new GamePlayer($gameid, $teamid, $goalie_playerid);
      $game_player->build(); //gets the game_playerid needed for the update
      $game_player->setGoalieYN(1);
      $update_players[$game_player->getPlayerId()] = $game_player;
    }

    // update the old goalie if checked and is not being removed
    if ($previous_goalie_playerid > 0 && !in_array($previous_goalie_playerid, $remove_team_players)) {
      $game_player = new GamePlayer($gameid, $teamid, $previous_goalie_playerid);
      $game_player->build(); //gets the game_playerid needed for the update
      $game_player->setGoalieYN(0);
      $update_players[$game_player->getPlayerId()] = $game_player;
    }
  }

  hockey_stats_game_add_players($add_players);
  hockey_stats_game_remove_players($remove_players);
  hockey_stats_game_update_players($update_players);
}

function hockey_stats_game_add_players($players) {
  foreach ($players as $game_player) {
    $game_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $game_player->save();
  }
}

function hockey_stats_game_remove_players($players) {
  foreach ($players as $game_player) {
    $game_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $game_player->delete();
  }
}

function hockey_stats_game_update_players($players) {
  foreach ($players as $game_player) {
    $game_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $game_player->save();
  }
}

function hockey_stats_game_goals($form, &$form_state) {
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_goals_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    
  }

  $form_state['redirect'] = array('base/game', );
}

function hockey_stats_game_penalties_landing($form, &$form_state) {
  
  $gameid = Utility::getCookieValue(Utility::COOKIE_GAMEID);
  
  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else if(isset($gameid)) {
    $game = new Game($gameid);
    $game->build();
  }
  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }
  
  Utility::setCookieValue(Utility::COOKIE_GAMEID, $game->getGameId());
  
  $header = array(
       'period' => t('Period'),
       'time' => t('Time'),
       'penalty' => t('Penalty'),
       'player' => t('Player'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();
  
  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new penalty'),
       '#href' => '#',
       '#attributes' => array('onclick' => 'penaltyAdd(0)', 'class' => 'preventDefault'),
  );
  
  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'penaltyTable',
       '#empty' => t('No Penalties Found'),
  );  
    
  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['gamePenaltyId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'gamePenaltyId'),
  );
  
  return $form;
}

function hockey_stats_game_penalties_add($form, &$form_state) {

  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }

  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }
  
  $application = new Application();
  
  $form['penalties'] = array(
       '#type' => 'fieldset',
       '#title' => t('Penalty for ' . $game->getGameDescription()),
  );
    
  // add new penalty
  $form['penalties']['period'] = array(
       '#type' => 'select',
       '#title' => t('Period'),
       '#options' => array(0 => '') + $application->getPeriodList(), 
  );
  
  $form['penalties']['time'] = array(
       '#type' => 'textfield',
       '#title' => t('Time'),
       '#size' => 5,
       '#maxlength' => 5,
  );
  
  $form['penalties']['seasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Player'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
  );
  
  $form['penalties']['penaltyId'] = array(
       '#type' => 'select',
       '#title' => t('Penalty'),
       '#options' => array(0 => '') + $application->getPenalties(),  
  );  
  
  $form['penalties']['duration'] = array(
       '#type' => 'select',
       '#title' => t('Penalty Duration'),
       '#options' => array(0 => '') + $application->getPenaltyDurationList(), 
  );
  
  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );
  
  $form['gamePenaltyId'] = array(
       '#type' => 'hidden',
       '#value' => 0,
  );
       
  // buttons
  $form['save'] = array(
       '#type' => 'submit',
       '#value' => 'Save and Add Another',
  );
    
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_penalties_add_validate($form, &$form_state) {
  $button = $form_state['input']['op'];

  if ($button == "Submit" || $button == "Save and Add Another") {

    $penalty = hockey_stats_build_penalty_from_form($form_state);
    $application = new Application();

    // period must be 1,2,3,OT (4)
    if (!$application->isValidPeriod($penalty->getPeriod())){
       form_set_error('period', 'Period is not valid');
    }

    //  time must be valid
    if (!Utility::isValidTime($penalty->getTime())) {
      form_set_error('time', 'Time is not valid (MM:SS format)');
    }

    // player must be selected
    if ($penalty->getSeasonTeamPlayerId() == 0) {
      form_set_error('seasonTeamPlayerId', 'Player is not valid');
    }

    // penalty must be selected
    if ($penalty->getPenaltyId() == 0) {
      form_set_error('penaltyId', 'Penatly is not valid');
    }

    // penalty duratin is required
    if ($penalty->getPenaltyDurationId() == 0) {
      form_set_error('duration', 'Penalty Duration is required');
    }  
  }
}

function hockey_stats_game_penalties_add_submit($form, &$form_state) {
  $button = $form_state['input']['op'];
    
  if ($button == "Submit" || $button == "Save and Add Another") {
    $penalty = hockey_stats_build_penalty_from_form($form_state);
    $penalty->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    
    $penalty_original = new GamePenalty($penalty->getGamePenaltyId());
    $penalty_original->build();

    if (!isset($penalty_original)) {
      $penalty_original = new GamePenalty();
    }

    if (!$penalty->equals($penalty_original)) {
      $penalty->save();
    }

    drupal_set_message(t('The penalty has been saved'));
  }
  
  $form_state['redirect'] = array('base/game/penalties', );
}


function hockey_stats_build_penalty_from_form(&$form_state) {

  $game_penaltyid = $form_state['input']['gamePenaltyId'];
  $gameid = $form_state['input']['gameId'];
  $period = $form_state['input']['period'];
  $time = trim($form_state['input']['time']);
  $season_team_playerid = $form_state['input']['seasonTeamPlayerId'];
  $penaltyid = $form_state['input']['penaltyId'];  
  $duration = trim($form_state['input']['duration']);

  $penalty = new GamePenalty($game_penaltyid, $gameid, $period, $time, 
    $season_team_playerid, $penaltyid, $duration);

  return $penalty;
}


