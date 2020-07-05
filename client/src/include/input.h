#pragma once
#include <string>

/**
 * Gets string information from the CLI user.
 *
 * @param [in] prompt   Will be printed to stdout
 *
 * @return The user's response from the stdin following the prompt.
 */
std::string get_input(std::string prompt);

/**
 * Gets string information from the CLI user,
 * while not showing to the screen the text.
 *
 * @param [in] prompt   Will be printed to stdout
 *
 * @return The user's response from the stdin following the prompt.
 */
std::string get_input_hidden(std::string prompt);

/**
 * Gets a boolean valuefrom the CLI user,
 * in the form of a Yes or No question,
 * which must be answered as one of "Y"/"N".
 *
 * @param [in] prompt   Will be printed to stdout
 *
 * @return true or false, on a valid answers.
 *
 * @throw std::runtime_error, on invalid answers.
 */
bool yes_or_no(std::string prompt);