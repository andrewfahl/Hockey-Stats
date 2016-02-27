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