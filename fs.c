// Filesystem module
// Copyright (C) 2015 Legimet
//
// This file is part of Duktape-nspire.
//
// Duktape-nspire is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Duktape-nspire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Duktape-nspire. If not, see <http://www.gnu.org/licenses/>.

#include "duktape.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#define FS_SET_STATS_INT_PROP(ctx, buf, idx, prop) \
do { \
    duk_push_int((ctx), (buf)->st_##prop); \
    duk_put_prop_string((ctx), (idx), #prop); \
} while (0)

#define FS_SET_STATS_DATE_PROP(ctx, buf, idx1, idx2, prop) \
do { \
    duk_get_prop_string((ctx), (idx1), "Date"); \
    duk_push_int((ctx), buf->st_##prop); \
    duk_new((ctx), 1); \
    duk_put_prop_string((ctx), (idx2), #prop); \
} while (0)

static duk_ret_t fs_err(duk_context *ctx, bool success, duk_idx_t callback, int callback_args) {
    if (duk_is_function(ctx, callback)) {
	duk_dup(ctx, callback);
	duk_insert(ctx, -callback_args);
    }
    if (!success) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "%s", strerror(errno));
	if (duk_is_function(ctx, callback)) {
	    duk_insert(ctx, -callback_args);
	}
    }
    if (duk_is_function(ctx, callback)) {
	if (success) {
	    duk_push_null(ctx);
	    if (duk_is_function(ctx, callback)) {
		duk_insert(ctx, -callback_args);
	    }
	}
	duk_call(ctx, callback_args);
    } else if (!success) {
	duk_throw(ctx);
    }
    if (callback_args) {
	return 0;
    } else {
	return 1;
    }
}

static duk_ret_t fs_rename(duk_context *ctx) {
    bool success = !rename(duk_require_string(ctx, 0), duk_require_string(ctx, 1));
    return fs_err(ctx, success, 2, 1);
}

static void fs_init_stats_obj(duk_context *ctx, struct stat *buf) {
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "Stats");
    duk_new(ctx, 0);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, dev);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, ino);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, mode);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, nlink);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, uid);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, gid);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, rdev);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, size);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, blksize);
    FS_SET_STATS_INT_PROP(ctx, buf, -2, blocks);
    duk_push_global_object(ctx);
    FS_SET_STATS_DATE_PROP(ctx, buf, -1, -3, atime);
    FS_SET_STATS_DATE_PROP(ctx, buf, -1, -3, mtime);
    FS_SET_STATS_DATE_PROP(ctx, buf, -1, -3, ctime);
    duk_swap(ctx, -2, -3);
    duk_pop_2(ctx);
}

static duk_ret_t fs_stat(duk_context *ctx) {
    struct stat buf;
    bool success = !stat(duk_require_string(ctx, 0), &buf);
    if (success) {
	fs_init_stats_obj(ctx, &buf);
    } else {
	duk_push_undefined(ctx);
    }
    return fs_err(ctx, success, 1, 2);
}

static duk_ret_t fs_stat_sync(duk_context *ctx) {
    struct stat buf;
    bool success = !stat(duk_require_string(ctx, 0), &buf);
    if (success) {
	fs_init_stats_obj(ctx, &buf);
    }
    return fs_err(ctx, success, DUK_INVALID_INDEX, 0);
}

static duk_ret_t fs_unlink(duk_context *ctx) {
    bool success = !unlink(duk_require_string(ctx, 0));
    return fs_err(ctx, success, 1, 1);
}

static duk_ret_t fs_rmdir(duk_context *ctx) {
    bool success = !rmdir(duk_require_string(ctx, 0));
    return fs_err(ctx, success, 1, 1);
}

static duk_ret_t fs_mkdir(duk_context *ctx) {
    bool success;
    if (duk_is_number(ctx, 1)) {
	success = !mkdir(duk_require_string(ctx, 0), duk_require_int(ctx, 1));
	return fs_err(ctx, success, 2, 1);
    } else {
	success = !mkdir(duk_require_string(ctx, 0), 0777);
	return fs_err(ctx, success, 1, 1);
    }
}

static duk_ret_t fs_close(duk_context *ctx) {
    bool success = !close(duk_require_int(ctx, 0));
    return fs_err(ctx, success, 1, 1);
}

static bool fs_open_get_flags(const char *s, int *flag) {
    int flag1 = 0;
    int flag2 = 0;

    switch (s[0]) {
	case 'r':
	    flag1 = O_RDONLY;
	    break;
	case 'w':
	    flag1 = O_WRONLY;
	    flag2 = O_CREAT | O_TRUNC;
	    break;
	case 'a':
	    flag1 = O_WRONLY;
	    flag2 = O_APPEND | O_TRUNC;
	    break;
	default:
	    return false;
    }

    switch (s[1]) {
	case '\0':
	    break;
	case '+':
	    if (s[2])
		return false;
	    flag1 = O_RDWR;
	    break;
	case 's':
	    if (flag1 != O_RDONLY || (s[2] != '\0' && s[2] != '+') ||
		(s[2] == '+' && s[3]))
		return false;
	    if (s[2] == '+')
		flag1 = O_RDWR;
	    break;
	case 'x':
	    if (flag1 != O_WRONLY || (s[2] != '\0' && s[2] != '+') ||
		(s[2] == '+' && s[3]))
		return false;
	    flag2 |= O_EXCL;
	    if (s[2] == '+')
		flag1 = O_RDWR;
	    break;
	default:
	    return false;
    }

    *flag = flag1 | flag2;

    return true;
}

static duk_ret_t fs_open(duk_context *ctx) {
    int fd;
    const char *path = duk_require_string(ctx, 0);
    const char *flags_str = duk_require_string(ctx, 1);
    int flags;

    if (!fs_open_get_flags(flags_str, &flags)) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "Unknown file open flag: %s", flags_str);
	duk_throw(ctx);
    }

    mode_t mode;
    duk_idx_t callback;
    if (duk_is_number(ctx, 2)) {
	mode = duk_get_int(ctx, 2);
	callback = 3;
    } else {
	mode = 0666;
	callback = 2;
    }
    fd = open(path, flags, mode);
    if (fd != -1) {
	duk_push_int(ctx, fd);
    } else {
	duk_push_undefined(ctx);
    }
    return fs_err(ctx, fd != -1, callback, 2);
}

static duk_ret_t fs_open_sync(duk_context *ctx) {
    int fd;
    const char *path = duk_require_string(ctx, 0);
    const char *flags_str = duk_require_string(ctx, 1);
    int flags;

    if (!fs_open_get_flags(flags_str, &flags)) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "Unknown file open flag: %s", flags_str);
	duk_throw(ctx);
    }

    mode_t mode;
    if (duk_is_number(ctx, 2)) {
	mode = duk_get_int(ctx, 2);
    } else {
	mode = 0666;
    }
    fd = open(path, flags, mode);
    if (fd != -1) {
	duk_push_int(ctx, fd);
    }

    return fs_err(ctx, fd != -1, DUK_INVALID_INDEX, 0);
}

static duk_ret_t fs_stats_check_mode(duk_context *ctx, mode_t mask) {
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "mode");
    if (duk_is_undefined(ctx, -1)) {
	duk_push_false(ctx);
    } else {
	if ((duk_get_int(ctx, -1) & S_IFMT) == mask) {
	    duk_push_true(ctx);
	} else {
	    duk_push_false(ctx);
	}
    }
    return 1;
}

static duk_ret_t fs_stats_is_file(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFREG);
}

static duk_ret_t fs_stats_is_dir(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFDIR);
}

static duk_ret_t fs_stats_is_blkdev(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFBLK);
}

static duk_ret_t fs_stats_is_chardev(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFCHR);
}

static duk_ret_t fs_stats_is_symlink(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFLNK);
}

static duk_ret_t fs_stats_isfifo(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFIFO);
}

static duk_ret_t fs_stats_issocket(duk_context *ctx) {
    return fs_stats_check_mode(ctx, S_IFSOCK);
}

static const duk_function_list_entry fs_stats_methods[] = {
    {"isFile", fs_stats_is_file, 0},
    {"isDirectory", fs_stats_is_dir, 0},
    {"isBlockDevice", fs_stats_is_blkdev, 0},
    {"isCharacterDevice", fs_stats_is_chardev, 0},
    {"isSymbolicLink", fs_stats_is_symlink, 0},
    {"isFIFO", fs_stats_isfifo, 0},
    {"isSocket", fs_stats_issocket, 0},
    {NULL, NULL, 0}
};

static duk_ret_t fs_stats_constructor(__attribute__((unused)) duk_context *ctx) {
    return 0;
}

static const duk_function_list_entry fs_funcs[] = {
    {"rename", fs_rename, 3},
    {"renameSync", fs_rename, 2},
    {"stat", fs_stat, 2},
    {"lstat", fs_stat, 2},
    {"statSync", fs_stat_sync, 1},
    {"lstatSync", fs_stat_sync, 1},
    {"unlink", fs_unlink, 2},
    {"unlinkSync", fs_unlink, 1},
    {"rmdir", fs_rmdir, 2},
    {"rmdirSync", fs_rmdir, 1},
    {"mkdir", fs_mkdir, DUK_VARARGS},
    {"mkdirSync", fs_mkdir, DUK_VARARGS},
    {"close", fs_close, 2},
    {"closeSync", fs_close, 1},
    {"open", fs_open, DUK_VARARGS},
    {"openSync", fs_open_sync, DUK_VARARGS},
    {NULL, NULL, 0}
};

duk_ret_t dukopen_fs(duk_context *ctx) {
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, fs_funcs);
    duk_push_c_function(ctx, fs_stats_constructor, 0);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, fs_stats_methods);
    duk_put_prop_string(ctx, -2, "prototype");
    duk_put_prop_string(ctx, -2, "Stats");
    return 1;
}
