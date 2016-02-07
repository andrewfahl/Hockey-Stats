<?php
/**
 * @file
 * Hockey Stats Module Generic Pages and Functions
 */

/**
 * Main landing page content
 */
function hockey_stats_home_page_content() {

    header('Content-type: text/plain; charset=UTF-8');
    header('Content-Disposition: inline');

    $html = "To Do:";
    $html .= '<ul>';
    $html .= '<li>Team Logos</li>';
    $html .= '<li>Team Placeholder (TBD, 1st Place...)</li>';
    $html .= '<li>Genaric Player (Sub, ringer....)</li>';
    $html .= '<li>Figure out how to restrict access</li>';
    $html .= '<li>Build Views in for front end</li>';
    $html .= '<li>Make sure all deletes cascade</li>';
    $html .= '<li>Add project to github</li>';
    $html .= '<li>Default Goalie on the attendance goalie drop downs (SeasonTeam)</li>';
    $html .= '</ul>';
    
    $html .= "Nice To Have:";
    $html .= '<ul>';
    $html .= '<li>Set Roster Search For Player functionality</li>';
    $html .= '<li>USA Hockey Info</li>';
    $html .= '<li>Payment Info</li>';
    $html .= '<li>The munu expanded all the time</li>';
    $html .= '<li>Build scord sheet from pdf</li>';
    $html .= '<li>Player Ratings</li>';
    $html .= '<li>Identify Team Captain (maybe with contact info)</li>';
    $html .= '</ul>';

    $html .= "Inquire about:";
    $html .= '<ul>';
    $html .= '<li>Navigaion: is the query string a problem?</li>';
    $html .= '<li>Set Roster All Players in mult columns or do we keep the formating out of the module?</li>';
    $html .= '<li>Do all of my tables in the DB need to start with the Module Name?</li>';
    $html .= '</ul>';
    
    //$html .= '<li></li>';
    return $html;
}