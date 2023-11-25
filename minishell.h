/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:30:26 by omathot           #+#    #+#             */
/*   Updated: 2023/11/25 23:34:07 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include "./lib/libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>

extern int	g_exit_status;

typedef struct s_post_quotes
{
	char	*content;
	bool	is_quotes;
	bool	have_to_expand;
}	t_post_quotes;

typedef struct s_lexer
{
	char	tokenid[3];
	char	*content;
	char	*cmd;
	char	*flags;
	char	*args;
	char	*file;
	int		possition;
	bool	execd;
}	t_lexer;

typedef struct s_list_of_quotes
{
	int	*single_quotes;
	int	*double_quotes;
	int	*i_temp;
}	t_list_of_quotes;

typedef struct s_pipedata
{
	char	***environ;
	int		og_in;
	int		og_out;
	int		input_fd;
	int		lex_count;
	int		fd[2];
	t_lexer	**lexer;
}	t_pipedata;

void				manage_signals(int mode);
void				exec(t_lexer *lexer);
void				piping(t_lexer **lexer);
char				*ft_strtok(char *str, const char *delim);
void				concat_path(char *buffer, const char *dir, const char *cmd);
char				*str_dup_until_index(char *str, int index);
int					executer(t_lexer **lexer, t_pipedata *data);
int					is_built_in(t_lexer **lexer, int lex_count);
char				*doll_to_num(char *str_og, int number_replace);
char				*here_doc_starter(char *wordlocking_for);
char				*sjoin_fr(char *s1, char *s2);
int					ft_char_find(char *str, const char *list_of_char);
char				*check_exisisting_env(char ***environment, char *name,
						char **value, int *i_og);
bool				ft_isspace(unsigned char c);
void				malloc_new_args_and_new_flags(int args_char_len,
						char **args, char **flags, int flags_char_len);
char				*handle_expand_doll(char *str);
int					copy_until_tokenid(int i, char *input_after_curser,
						char **destination);
char				*remove_front_spaces(char *str);
char				*remove_back_spaces(char *str);
int					count_token(char *input);
int					write_the_right_token(int i, char *input_after_curser,
						char tokenid[3]);
void				initiate_values_to_zero_null(t_lexer **lexer, int i);
bool				is_a_token_id(char *input);
int					copy_until_space(int i, char *input_after_curser,
						char **destination);
void				move_cmd_if_speacific_tokenid(t_lexer **previus_lexer,
						t_lexer **current_lexer, int i);
void				parse_with_quotes_none_quotes_assigning_if_token(char
						**curent_content, int *j, t_lexer ***lexer, int *i);
void				move_file_name_to_file_and_comand_back_p2(t_lexer *lexer,
						int i);
int					parse_until_token_id(char *input,
						int current_lex, t_lexer **lexer, int curser);
void				move_flags_from_args_to_flags(t_lexer *lexer);
void				move_file_name_to_file_and_comand_back(
						t_lexer *lexer_previous, t_lexer *lexer);
char				**get_list_of_tokenid(void);
void				count_num_dash_and_flags_char_len(char **str,
						int i, int *num_of_dash, int *flags_char_len);
bool				ft_isspace(unsigned char c);
void				check_quotes(char *str);
int					initialize_and_checks(t_post_quotes ***content,
						bool quotes_test, char **str);
void				count_and_locate_quotes_init(int *num_single_quotes,
						int *num_double_quotes, char *str);
t_list_of_quotes	*count_and_locate_quotes(char *str);
void				set_bools_riun(bool *expandable,
						bool *in_quotes, bool to_expandable, bool to_quotes);
int					return_index_until_new(t_list_of_quotes *l_q,
						int i_cc, bool *exp, bool *i_q);
char				*get_env_of_valus_str(char *str);
void				lexer_free(t_lexer **lexer);
bool				is_str_export(char *str);
int					main(void);
char				*read_user_input(bool quotes_test);
char				is_prompt_empty(char *str);
t_lexer				**main_parser(char *str);
void				check_quotes_diff(char **str_og, t_post_quotes ***content);
t_lexer				**parser_with_quotes(t_post_quotes **content);
bool				check_export_for_quotes(t_post_quotes	***content,
						t_lexer ***lexer);
char				**get_export_var(char *arg_of_export);
bool				check_unset_for_quotes(t_post_quotes	***content,
						t_lexer ***lexer);
void				free_double_array(char **list_of_tokenid);
void				pipe_export(t_lexer ***lexer);
int					lexer_with_quotes(t_lexer ***lexer,
						t_post_quotes ***content, char **str);
int					noquotes_into_pipes(t_lexer ***lexer);
void				finish_output(t_lexer **lexer);
void				ascii_sort(char **environment);
char				***get_env(void);
void				set_env(char *name, char *value, char ***environment);
void				print_export(char ***environment);
void				free_content(t_post_quotes **content);
bool				is_str_unset(char *str);
bool				is_all_space(char *str);
char				**split_spaces(char *str);
char				**remalloc_and_dup(char **all_var_rm, int all_var_rm_total);
int					unset_env(char *name, char ***environment);
int					checks_andset_i(t_lexer ***lexer,
						t_post_quotes ***content, int *i);
int					count_len_split_world(char *str);
int					finish_or_malloc_word(char *str,
						char ***s_p, int *wc);
bool				check_if_array_str_is_empty(char **array_str);
void				export_content_freeur(char ***export_content);
char				get_last_char(char *str);
bool				check_export_for_quotes_check_start(
						t_lexer ***lexer, t_post_quotes	***content);
int					counte_num_new_var(t_post_quotes **content);
int					get_num_export_con(char *arg);
char				*ft_strdup_until_space(char *str);
int					ft_strtok_line_help(char **str_reminder,
						int *index_unter_delim, int *curser);
char				*get_cmd_path(const char *cmd, t_pipedata *data);
int					redirection_handler(t_lexer *lexer);
void				export_parent_management(t_lexer ***lexer,
						t_pipedata *data, int pid);
void				initialize_pipedata(t_pipedata *data);
int					pipe_export_error(t_lexer ***lexer);
int					print_if_correct(t_lexer ***lexer);
char				**mk_args_for_proc(t_pipedata *data);
int					are_there_more_cmds(t_lexer **lexer, int current);
bool				execute_child_process(t_pipedata *data);
int					parent_management(t_pipedata *data,
						t_lexer **lexer, int pid);

#endif