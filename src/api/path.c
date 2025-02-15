#include "minishell.h"

/*	appends / to path if it's not already there
	~ is expanded to $HOME
*/
char	*new_path_resolved(char *path, t_dict *env)
{
	char	*new;

	new = new_str(path);
	path_replace_tilde(&new, env);
	if (!is_str_last(new, '/'))
		ft_str_replace(&new, new_str_join((char *[]){
				new, "/", NULL}, '\0'));
	return (new);
}

static char	**new_raw_path(t_dict *env)
{
	const char	*pathstr = dict_get(env, "PATH");

	if (!pathstr)
		return (NULL);
	return (new_str_split(pathstr, ':'));
}

//	returns resolved array of paths
static char	**new_path(t_dict *env)
{
	int		i;
	char	**raw;
	char	**path;

	raw = new_raw_path(env);
	path = new_arr(NULL);
	i = -1;
	while (raw[++i])
		ft_arr_append_free(&path, new_path_resolved(raw[i], env));
	del_arr(raw);
	return (path);
}

/*	returns array of candidates
	ex: "echo"
	-> [ 1] "/usr/local/bin/echo"
	-> [ 2] "/usr/bin/echo"
	-> [14] "/Users/youkim/.brew/bin/echo"
	...
*/
char	**new_names_from_path(char *name, t_dict *env)
{
	int		i;
	char	**new;
	char	**path;

	path = new_path(env);
	new = new_arr(NULL);
	i = -1;
	while (path[++i])
		ft_arr_append_free(&new, new_str_join((char *[]){
				path[i], name, NULL}, '\0'));
	del_arr(path);
	return (new);
}
