#include "../../include/minishell.h"

void start_message(void)
{
    printf("\n______________________________________________________________________________\n");
    printf("   _____  .__       .__       .__           .__  .__       _____     ____  \n");
    printf("  /     \\ |__| ____ |__| _____|  |__   ____ |  | |  |     /      \\  |  o |\n");
    printf(" /  \\ /  \\|  |/    \\|  |/  ___/  |  \\_/ __ \\|  | |  |    |        |/ ___\\|\n");
    printf("/    Y    \\  |   |  \\  |\\___ \\|   Y  \\  ___/|  |_|  |__  |_________/ \n");
    printf("\\____|__  /__|___|  /__/____  >___|  /\\___  >____/____/  |_|_| |_|_|\n");
    printf("        \\/        \\/        \\/     \\/     \\/           [by xueyang & jwardeng]\n");
    printf("______________________________________________________________________________\n");
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_put_to_fd(char *str, int fd)
{
	if (write(fd, str, ft_strlen(str)) < 0)
	{
		perror("write error");
		return (1);
	}
	return (0);
}

char	*ft_env_substr(char const *s, unsigned int start, size_t len, t_gc *gc)
{
	char	*sub;
	size_t	count;

	if (start >= ft_strlen(s))
	{
		sub = gc_malloc(gc, ENV, 1);
		if (sub == NULL)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	sub = gc_malloc(gc, ENV, len + 1);
	if (sub == NULL)
		return (NULL);
	count = 0;
	while (count < len)
	{
		sub[count] = s[start + count];
		count++;
	}
	sub[count] = '\0';
	return (sub);
}

char	*ft_env_strdup(const char *src, t_gc *gc)
{
	char	*address;
	char	*old_dest;
	int		len;

	len = 0;
	while (src[len] != '\0')
	{
		len++;
	}
	address = gc_malloc(gc, ENV, len + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (*src != '\0')
	{
		*address = *src;
		address++;
		src++;
	}
	*address = '\0';
	return (old_dest);
}

char	*ft_env_strjoin(char const *s1, char const *s2, t_gc *gc)
{
	int		i;
	int		j;
	char	*new;

	new = gc_malloc(gc, ENV, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (new == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}
