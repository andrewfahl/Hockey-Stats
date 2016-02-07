<?php
/**
 * @file
 * Info for the views module
 * 
 */

/**
 * Implements hook_views_data().
 */
function hockey_stats_views_data() {

  $data = array();

  // location 
  $data['location']['table']['group'] = t('Hockey Stats');

  $data['location']['table']['base'] = array(
       'title' => t('Locations'),
       'help' => t('Contains the locations the games are played'),
  );

  $data['location']['locationId'] = array(
       'title' => t('Location ID'),
       'help' => t('The Location ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['location']['name'] = array(
       'title' => t('Name'),
       'help' => t('The Location Name'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['location']['address'] = array(
       'title' => t('Address'),
       'help' => t('The Location Address'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['location']['address2'] = array(
       'title' => t('Address2'),
       'help' => t('The Location Address2'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['location']['city'] = array(
       'title' => t('City'),
       'help' => t('The Location City'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['location']['state'] = array(
       'title' => t('State'),
       'help' => t('The Location State'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['location']['zip'] = array(
       'title' => t('Zip'),
       'help' => t('The Location Zip Code'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  // league
  $data['league']['table']['group'] = t('Hockey Stats Base');

  $data['league']['table']['base'] = array(
       'title' => t('League'),
       'help' => t('Contains the league data'),
  );

  $data['league']['leagueId'] = array(
       'title' => t('League ID'),
       'help' => t('The League ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['league']['name'] = array(
       'title' => t('Name'),
       'help' => t('The League Name'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['league']['sequence'] = array(
       'title' => t('Sort Order'),
       'help' => t('The League sort order'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  // season
  $data['season']['table']['group'] = t('Hockey Stats Base');

  $data['season']['table']['base'] = array(
       'title' => t('Season'),
       'help' => t('Contains the season data'),
  );

  $data['season']['seasonId'] = array(
       'title' => t('Season ID'),
       'help' => t('The Season ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['season']['name'] = array(
       'title' => t('Name'),
       'help' => t('The Season Name'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['season']['startDateTime'] = array(
       'title' => t('Start Date'),
       'help' => t('The Season Start Date'),
       'field' => array(
            'handler' => 'views_handler_field_date',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_date',
       ),
  );

  $data['season']['table']['join'] = array(
       'league' => array(
            'left_field' => 'leagueId',
            'field' => 'leagueId',
       ),
  );

  // team
  $data['team']['table']['group'] = t('Hockey Stats Base');

  $data['team']['table']['base'] = array(
       'title' => t('Team'),
       'help' => t('Contains the team data'),
  );

  $data['team']['teamId'] = array(
       'title' => t('Team ID'),
       'help' => t('The Team ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['team']['name'] = array(
       'title' => t('Name'),
       'help' => t('The Team Name'),
       'field' => array(
            'handler' => 'views_handler_field',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_string',
       ),
  );

  $data['team']['activeYN'] = array(
       'title' => t('Team Is Active'),
       'help' => t('The Team Is Active'),
       'field' => array(
            'handler' => 'views_handler_field_boolean',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
       ),
  );

  // seasonteam
  $data['seasonteam']['table']['group'] = t('Hockey Stats Base');

  $data['seasonteam']['table']['base'] = array(
       'title' => t('Season Team'),
       'help' => t('Contains the teams that are in the season'),
  );

  $data['seasonteam']['seasonTeamId'] = array(
       'title' => t('Season Team ID'),
       'help' => t('The Season Team ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['seasonteam']['teamId'] = array(
       'title' => t('Season Team: Team ID'),
       'help' => t('The Team ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['seasonteam']['seasonId'] = array(
       'title' => t('Season Team: Season ID'),
       'help' => t('The Season ID'),
       'field' => array(
            'handler' => 'views_handler_field_numeric',
       ),
       'sort' => array(
            'handler' => 'views_handler_sort',
       ),
       'filter' => array(
            'handler' => 'views_handler_filter_numeric',
       ),
  );

  $data['seasonteam']['table']['join'] = array(
       'season' => array(
            'left_field' => 'seasonId',
            'field' => 'seasonId',
       ),
       'team' => array(
            'left_field' => 'teamId',
            'field' => 'teamId',
       ),
  );

  return $data;
}
