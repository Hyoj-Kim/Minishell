#include "minishell.h"

static t_res	error_unclosed(char c, t_scan_data *data)
{
	char	*quote_str;

	quote_str = (char []){'(', c, ')', ':', ' ', '\0'};
	return (free_n_return(&data->buf, error_with_exitcode((char *[]){
				BRED, MINISHELL, QUOTE_ERROR, quote_str,
				MULTILINE_ERROR, END, NULL}, data->env, 2)));
}

static t_res	scan_last_check(t_list **scan_list, t_scan_data *data)
{
	char	last_quote;

	if (is_quotes_open(&last_quote, data->buf))
		return (error_unclosed(last_quote, data));
	buf_to_list(scan_list, &data->buf);
	free(data->buf);
	return (OK);
}

static t_res	scanner_loop(t_list **scan_list, t_scan_data *data)
{
	t_res	scan_res;

	while (data->line[++(data->idx)])
	{
		if (whitespace_scan(scan_list, data) == OK)
			continue ;
		scan_res = dollar_scan(scan_list, data);
		if (scan_res == OK)
			continue ;
		if (scan_res == ERR)
			return (ERR);
		scan_res = metachar_scan(scan_list, data);
		if (scan_res == OK)
			continue ;
		if (scan_res == ERR)
			return (ERR);
		ft_str_append(&data->buf, data->line[data->idx]);
	}
	return (scan_last_check(scan_list, data));
}

/*
	scan line and create scan_list
	- ERR: syntax error, unclosed error
*/
t_res	scanner(t_list **scan_list, char *line, t_dict *env)
{
	t_scan_data	data;

	data.line = line;
	data.idx = -1;
	data.env = env;
	data.type = CMD;
	data.buf = new_str("");
	if (scanner_loop(scan_list, &data) == OK)
		return (OK);
	return (ERR);
}