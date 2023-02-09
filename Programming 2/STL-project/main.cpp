/*  COMP.CS.100 Project 2: GAME STATISTICS
 * ===============================
 * EXAMPLE SOLUTION
 * ===============================
 *
 * Acts as a game statistics with n commands:
 * ALL_GAMES - Prints all known game names
 * GAME <game name> - Prints all players playing the given game
 * ALL_PLAYERS - Prints all known player names
 * PLAYER <player name> - Prints all games the given player plays
 * ADD_GAME <game name> - Adds a new game
 * ADD_PLAYER <game name> <player name> <score> - Adds a new player with the
 * given score for the given game, or updates the player's score if the player
 * already playes the game
 * REMOVE_PLAYER <player name> - Removes the player from all games
 * ok_line - Checks if there are three and only three elements in the read file
 * print_games - Prints all games
 * print_names - Prints all players
 *
 * The data file's lines should be in format game_name;player_name;score
 * Otherwise the program execution terminates instantly (but still gracefully).
 *
 *
 * Programms writers:
 * Name: Joel Niskanen & Eetu
 * Student number: 50272794
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

using std::string, std::vector, std::cout, std::endl, std::cin,
std::ifstream, std::set, std::set, std::map;
using GAMES = std::map<string, std::map<string, int>>;


// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split(const std::string& str, char delim = ';')
{
    std::vector<std::string> result = {""};
    bool inside_quatation = false;
    for ( auto current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quatation = not inside_quatation;
        }
        else if ( current_char == delim and not inside_quatation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }
    return result;
}


// Check that the vector has three
// elements separated by semicolon
bool ok_line(vector<string> const &line_parts){
    return line_parts.size() == 3
           && not line_parts.at(0).empty()
           && not line_parts.at(1).empty();
}

// Function reads the input file
bool read_input_file(GAMES &scoreboard){
    string file_input = "";
    cout << "Give a name for input file: ";
    getline(cin, file_input);
    ifstream file(file_input);
    if(not file){
        cout << "Error: File could not be read." << endl;
        return false;
    }
    string row = "";
    vector<string> line_parts;
    while(getline(file, row)){
        line_parts = split(row);
        if(not ok_line(line_parts)){
            cout << "Error: Invalid format in file." << endl;
            file.close();
            return false;
        }
        //0: game, 1: player, 2: score
        string gamename = line_parts.at(0),
               player = line_parts.at(1),
               score_str = line_parts.at(2);
        // If game in the scoreboard
        if(scoreboard.find(gamename) == scoreboard.end()){
            scoreboard.insert({gamename, {}});
        }
        // Add player to the scoreboard
        scoreboard.at(gamename).insert({player, stoi(score_str)});
    }
    return true;
}

// Print all games in alphabetical order
void print_games(GAMES &scoreboard){
    cout << "All games in alphabetical order:" << endl;
    for(auto& entry : scoreboard){
        cout << entry.first << endl;
    }
}

// Prints all players in alphabetical order
void print_names(GAMES &scoreboard){
    cout << "All players in alphabetical order:" << endl;
    // Set for all players
    set <string> names;
    for(auto& it : scoreboard){
        map<string, int> &internal_map = it.second;
        for(auto& it2 : internal_map){
            names.insert(it2.first);
        }
    }
    for(auto& it : names){
        cout << it << endl;
    }
}

// Add new game
void add_game(const string game, GAMES &scoreboard) {
    if(scoreboard.find(game) == scoreboard.end()){
        scoreboard.insert({game, {}});
        cout << "Game was added." << endl;
    } else {
        cout << "Error: Already exists." << endl;
    }
}

// Lists all the games specific person plays
void player(const string name, GAMES &scoreboard) {
    set <string> game;
    for(auto& it : scoreboard){
        map<string, int> &internal_map = it.second;
        for(auto& it2 : internal_map){
            if(it2.first == name){
                game.insert(it.first);
            }
        }
    }
    if(game.size() == 0){
        cout << "Error: Player could not be found." << endl;
    } else {
        cout << "Player " << name << " playes the following games:" << endl;
        for(auto& it : game){
            cout << it << endl;
        }
    }
}

// Removes player's information from the scoreboard
void remove_player(const string player, GAMES &scoreboard){
    bool seen = false;
    for(auto it : scoreboard){
        map<string, int> &internal_map = it.second;
        for(auto it2 : internal_map){
            if(it2.first == player){
                scoreboard.at(it.first).erase(it2.first);
                seen = true;
            }
        }
    } if(!seen){
        cout << "Error: Player could not be found." << endl;
    } else {
        cout << "Player was removed from all games." << endl;
    }
}

// Adds new player
void add_player(const string game, const string name,
                const string points, GAMES &scoreboard){
    set <string> already_exists;
        for(auto& it : scoreboard){
            map<string, int> &internal_map = it.second;
            for(auto& it2 : internal_map){
                if(it2.first == name){
                    already_exists.insert(it.first);
                }
            }
        }
        // If the input name doesn't exist in the scoreboard
        // print error
        if(scoreboard.find(game) == scoreboard.end()){
            cout << "Error: Game could not be found." << endl;
            return;
        }
        // If the player already exists in the scoreboard
        // add new game and score
        if(already_exists.size() == 0){
            scoreboard.at(game).insert({name, stoi(points)});
            cout << "Player was added." << endl;
        } else {
            // If the player already plays the game
            // update the score
            scoreboard.at(game).at(name) = stoi(points);
            cout << "Player was added." << endl;
        }
    scoreboard.at(game).insert({name, stoi(points)});
}


// Print all the players playing specific game
void game(string gamename, GAMES &scoreboard){
    map<int, set<string>> points;
    for(auto& it : scoreboard){
        if(gamename == it.first){
            map<string, int> &internal_map = it.second;
            for(auto& it2 : internal_map){
                map<int, set<string>>::iterator it3 = points.find(it2.second);
                if(it3 != points.end()){
                    points.at(it2.second).insert(it2.first);
                } else {
                    points.insert({it2.second, {it2.first}});
                }
            }
        }
    }
    if(scoreboard.find(gamename) == scoreboard.end()){
        cout << "Error: Game could not be found." << endl;
    } else {
        cout << "Game " << gamename << " has these scores and players, listed in ascending order:" << endl;
        for(auto& it : points){
            cout << it.first << " : ";
            for(auto it2 = it.second.begin(); it2 != it.second.end(); it2++){
                if(it2 != it.second.begin()){
                    cout << ", ";
                }
                cout << *it2;
            }   cout << endl;
        }
    }
}

// Reads the user input
bool input(string input_line, GAMES &scoreboard){
    vector<string> parts;
    parts = split(input_line, ' ');
    // If the command has only a single word
    if(parts.size() == 1){
        if(input_line == "QUIT" || input_line == "quit") {
            return false;
        } else if(input_line == "ALL_GAMES" || input_line == "all_games") {
            print_games(scoreboard);
        } else if(input_line == "ALL_PLAYERS" || input_line == "all_players") {
            print_names(scoreboard);
        } else {
            cout << "Error: Invalid input." << endl;
        }
    // If the command has two words
    } else if(parts.size() == 2) {
        string command = parts.at(0),
               gamename = parts.at(1);
        if(command == "GAME" || command == "game") {
            game(gamename, scoreboard);
        } else if(command == "ADD_GAME" || command == "add_game") {
            add_game(gamename, scoreboard);
        } else if(command == "PLAYER" || command == "player"){
            player(gamename, scoreboard);
        } else if(command == "REMOVE" || command == "remove"){
            remove_player(gamename, scoreboard);
        } else {
            cout << "Error: Invalid input." << endl;
        }
    // If argument has two words without phrentasies
    } else if(parts.size() == 3) {
        string command = parts.at(0),
               gamename = parts.at(1);
        if(command == "GAME" || command == "game"){
            game(gamename, scoreboard);
        } else if(command == "PLAYER" || command == "player") {
            player(gamename, scoreboard);
        } else {
            cout << "Error: Invalid input." << endl;
        }
    // If the command has four words
    } else if(parts.size() == 4){
        string command = parts.at(0),
               gamename = parts.at(1),
               player = parts.at(2),
               score_str = parts.at(3);
       if(command == "ADD_PLAYER" || command == "add_player"){
            add_player(gamename, player, score_str, scoreboard);
       }
    } else {
        cout << "Error: Invalid input." << endl;
    }
    return true;
}

int main()
{
    GAMES scoreboard;
    if(not read_input_file(scoreboard)) {
        return EXIT_FAILURE;
    }
    string input_line = "";
    while(true){
        cout << "games> ";
        getline(cin, input_line);
        if(not input(input_line, scoreboard)){
            break;
        }
    }

    return EXIT_SUCCESS;
}

