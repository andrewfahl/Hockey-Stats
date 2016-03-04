jQuery(document).ready(function () {
    jQuery('.setFocusOnLoad').focus();
    jQuery('.preventDefault').click(function(event){
        event.preventDefault();
        }); 
});

function leagueEdit(leagueId) {
    jQuery('#formAction').val('edit');
    jQuery('#leagueId').val(leagueId);
    
    leaguePostTo();
}

function leagueDelete(leagueId) {
    jQuery('#formAction').val('delete');
    jQuery('#leagueId').val(leagueId);
    
    leaguePostTo();
}

function leaguePostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-league-landing').attr('action', '/?q=base/league/'+goTo);
    jQuery('#hockey-stats-league-landing').submit();
}

function seasonEdit(seasonId) {
    jQuery('#formAction').val('edit');
    jQuery('#seasonId').val(seasonId);
    
    seasonPostTo();
}

function seasonEditTeams(seasonId) {
    jQuery('#formAction').val('editteams');
    jQuery('#seasonId').val(seasonId);
    seasonPostTo();
}

function seasonDelete(seasonId) {
    jQuery('#formAction').val('delete');
    jQuery('#seasonId').val(seasonId);
    
    seasonPostTo();
}

function seasonPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-season-landing').attr('action', '/?q=base/season/'+goTo);
    jQuery('#hockey-stats-season-landing').submit();
}

function teamEdit(teamId) {
    jQuery('#formAction').val('edit');
    jQuery('#teamId').val(teamId);
    
    teamPostTo();
}

function teamDelete(teamId) {
    jQuery('#formAction').val('delete');
    jQuery('#teamId').val(teamId);
    
    teamPostTo();
}

function setRoster(teamId) {
    jQuery('#formAction').val('setroster');
    jQuery('#teamId').val(teamId);
    
    teamPostTo();
}

function teamAssignNumbers(teamId) {
    jQuery('#formAction').val('assignnumbers');
    jQuery('#teamId').val(teamId);
    
    teamPostTo();
}

function teamPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-team-landing').attr('action', '/?q=base/team/'+goTo);
    jQuery('#hockey-stats-team-landing').submit();
}

function locationEdit(locationId) {
    jQuery('#formAction').val('edit');
    jQuery('#locationId').val(locationId);
    
    locationPostTo();
}

function locationDelete(locationId) {
    jQuery('#formAction').val('delete');
    jQuery('#locationId').val(locationId);
    
    locationPostTo();
}

function locationPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-location-landing').attr('action', '/?q=base/location/'+goTo);
    jQuery('#hockey-stats-location-landing').submit();
}

function gameEdit(gameId) {
    jQuery('#formAction').val('edit');
    jQuery('#gameId').val(gameId);
    
    gamePostTo();
}

function gameDelete(gameId) {
    jQuery('#formAction').val('delete');
    jQuery('#gameId').val(gameId);
    
    gamePostTo();
}

function gameGoals(gameId) {
    jQuery('#formAction').val('goals');
    jQuery('#gameId').val(gameId);
    
    gamePostTo();
}

function gamePenalties(gameId) {
    jQuery('#formAction').val('penalties');
    jQuery('#gameId').val(gameId);
    
    gamePostTo();
}

function gameAttendance(gameId) {
    jQuery('#formAction').val('attendance');
    jQuery('#gameId').val(gameId);
    
    gamePostTo();
}

function gamePostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-game-landing').attr('action', '/?q=base/game/'+goTo);
    jQuery('#hockey-stats-game-landing').submit();
}

function playerEdit(playerId) {
    jQuery('#formAction').val('edit');
    jQuery('#playerid').val(playerId);
    
    playerPostTo();
}

function playerDelete(playerId) {
    jQuery('#formAction').val('delete');
    jQuery('#playerid').val(playerId);
    
    playerPostTo();
}

function playerPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-player-landing').attr('action', '/?q=base/player/'+goTo);
    jQuery('#hockey-stats-player-landing').submit();
}

function penaltyEdit(gamePenaltyId) {
    jQuery('#formAction').val('edit');
    jQuery('#gamePenaltyId').val(gamePenaltyId);
    penaltyPostTo();
}

function penaltyDelete(gamePenaltyId) {
    jQuery('#formAction').val('delete');
    jQuery('#gamePenaltyId').val(gamePenaltyId);
    penaltyPostTo();
}

function penaltyAdd(gamePenaltyId) {
    jQuery('#formAction').val('add');
    jQuery('#gamePenaltyId').val(gamePenaltyId);
    penaltyPostTo();
}

function penaltyPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-game-penalties-landing').attr('action', '/?q=base/game/penalties/'+goTo);
    jQuery('#hockey-stats-game-penalties-landing').submit();    
}

function goalEdit(gameGoalId) {
    jQuery('#formAction').val('edit');
    jQuery('#gameGoalId').val(gameGoalId);
    goalPostTo();
}

function goalDelete(gameGoalId) {
    jQuery('#formAction').val('delete');
    jQuery('#gameGoalId').val(gameGoalId);
    goalPostTo();
}

function goalAdd(gameGoalId) {
    jQuery('#formAction').val('add');
    jQuery('#gameGoalId').val(gameGoalId);
    goalPostTo();
}

function goalPostTo() { 
    var goTo = jQuery('#formAction').val();

    jQuery('#hockey-stats-game-goals-landing').attr('action', '/?q=base/game/goals/'+goTo);
    jQuery('#hockey-stats-game-goals-landing').submit();    
}

function setCookieSeasonId(seasonId) {
    jQuery.cookie("seasonid", seasonId, {expires: 30});
}

function setCookieLeagueId(leagueId) {
    // if the league id changed destroy the season cookie
    if(leagueId != jQuery.cookie("leagueid")) {
        document.cookie = "seasonid=; expires=Thu, 01 Jan 1970 00:00:00 UTC"; 
    }
    
    jQuery.cookie("leagueid", leagueId, {expires: 30});
}