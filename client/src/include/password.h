#pragma once

/**
 * RAII class that as a side effect of its initalisation,
 * stops the terminal from showing what the user is printing,
 * protecting the user from shoulder surfing,
 * or from unwanted screenshots by malware.
 */
class SilentTerminal final {
public:
  /**
   * Constructor.
   * Makes the terminal not echo the user's typing.
   */
  SilentTerminal();
  /**
   * Destructor.
   * Cancels out the effect of the constructor.
   */
  ~SilentTerminal();
};