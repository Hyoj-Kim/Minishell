#ifndef BUILTIN_H
# define BUILTIN_H

//@func
/*
** < env.c > */

void	env_set(t_dict *env, char *str);
t_dict	*new_env(char *envp[]);
void	env_print(t_dict *env);
char	**new_env_to_envp(t_dict *env);
/*
** < path.c > */

char	**new_path(t_dict *env);
void	path_print(char **path);
#endif