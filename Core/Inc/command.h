/**
  ******************************************************************************
  * @file           : command.h
  * @brief          : Command Processor Initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  * Much of the command processor code is credited towards Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
  * The commands themselves are different, but the actual processing of the commands will not be credited towards the final project.
  *
  *********************************
  */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#define MAX_STR_LEN 256

void accumulate_line(char *s);
void unknown_command(int argc, char argv[][MAX_STR_LEN]);
void handle_author(int argc, char argv[][MAX_STR_LEN]);
void handle_test(int argc, char argv[][MAX_STR_LEN]);
void handle_character(int argc, char argv[][MAX_STR_LEN]);
void process_command(char *input);

#endif /* INC_COMMAND_H_ */
