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
    $html .= '<li>Goals Page</li>';
    $html .= '<li>Penalties (currently on add new link)</li>';
    $html .= '<li>Figure out how to restrict access</li>';
    $html .= '<li>Build Views in for front end</li>';
    $html .= '<li>Make sure all deletes cascade</li>';
    $html .= '<li>Cancel Buttons shouldnt do a post back</li>';
    $html .= '<li>Make sure blanks in text fields are saved as NULL (Game notes)</li>';
    $html .= '<li>Move validation into the entities</li>';
    $html .= '<li>Sort Games in Season better.  Maybe push the completed games to the bottom in a different table</li>';
    $html .= '</ul>';
    
    $html .= "Think About:";
    $html .= '<ul>';
    $html .= '<li>Save Penalty Duration in Seconds Might make summing penalty minutes easier</li>';
    $html .= '</ul>';
     
    $html .= "Nice To Have:";
    $html .= '<ul>';
    $html .= '<li>Team Placeholder (TBD, 1st Place...)</li>';
    $html .= '<li>Genaric Player (Sub, ringer....)</li>';    
    $html .= '<li>Set Roster Search For Player functionality</li>';
    $html .= '<li>USA Hockey Info</li>';
    $html .= '<li>Payment Info</li>';
    $html .= '<li>The munu expanded all the time</li>';
    $html .= '<li>Build score sheet from pdf</li>';
    $html .= '<li>Player Ratings</li>';
    $html .= '<li>Identify Team Captain (maybe with contact info)</li>';
    $html .= '<li>Cleanup the js</li>';
    $html .= '<li>Players on suspension</li>';
    $html .= '<li>Players awards section</li>';
    $html .= '<li>Build a player view model so we can stop adding first and last name fields every where</li>';
    $html .= '</ul>';

    $html .= "Inquire about:";
    $html .= '<ul>';
    $html .= '<li>Navigation: is the query string a problem?</li>';
    $html .= '<li>Set Roster All Players in mult columns or do we keep the formating out of the module?</li>';
    $html .= '<li>Do all of my tables in the DB need to start with the Module Name?</li>';
    $html .= '<li>Will views work with datetime fields?</li>';
    $html .= '</ul>';
    
    //$html .= '<li></li>';
    return $html;
}