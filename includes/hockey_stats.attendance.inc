<?php

function hockey_stats_game_attendance_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_attendance');
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