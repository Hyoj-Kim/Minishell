#ifndef STRING_H
# define STRING_H

//@func
/*
** < array.c > */

t_res	ft_arr_append(char **parr[], char *str);
t_res	ft_arr_extend(char **parr[], char *src[]);
int		ft_arr_len(char **arr);
void	del_arr(char **arr);
/*
** < join.c > */

char	*new_str_join(char **arr, char delimiter);
/*
** < new.c > */

char	*new_str(const char *src);
int		ft_str_append(char **pstr, char *src);
/*
** < split.c > */

char	**new_str_split(char const *str, char delimitor);
/*
** < util.c > */

int		ft_strlen(const char *str);
bool	is_str_equel(const char *str1, const char *str2);
#endif