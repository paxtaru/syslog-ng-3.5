/*
 * Copyright (c) 2002, 2003, 2004 BalaBit IT Ltd, Budapest, Hungary
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Note that this permission is granted for only version 2 of the GPL.
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include "syslog-ng.h"

#include <sys/types.h>

struct _LogSourceGroup;
struct _LogDestGroup;
struct _LogFilterRule;
struct _LogConnection;
struct _LogCenter;
struct _LogTemplate;


/* configuration data kept between configuration reloads */
typedef struct _PersistentConfig PersistentConfig;

/* configuration data as loaded from the config file */
typedef struct _GlobalConfig
{
  gchar *filename;
  
  gint stats_freq;
  gint mark_freq;
  gint sync_freq;
  gboolean chain_hostnames;
  gboolean keep_hostname;
  gboolean use_time_recvd;
  gboolean use_fqdn;
  gboolean use_dns;
  gboolean use_dns_cache;
  gint dns_cache_size, dns_cache_expire, dns_cache_expire_failed;
  gint time_reopen;
  gint time_reap;

  gint log_fifo_size;
  gint log_fetch_limit;
  gint log_iw_size;
  gint log_msg_size;

  gint follow_freq;
  gboolean create_dirs;
  uid_t file_uid;
  gid_t file_gid;
  mode_t file_perm;
  
  uid_t dir_uid;
  gid_t dir_gid;
  mode_t dir_perm;

  gboolean keep_timestamp;  
  gint ts_format;
  gint tz_convert;
  
  gchar *file_template_name;
  gchar *proto_template_name;
  
  struct _LogTemplate *file_template;
  struct _LogTemplate *proto_template;
  
  /* */
  GHashTable *sources;
  GHashTable *destinations;
  GHashTable *filters;
  GHashTable *templates;
  GPtrArray *connections;
  
  struct _LogCenter *center;
  
} GlobalConfig;

extern GlobalConfig *configuration;

void cfg_add_source(GlobalConfig *configuration, struct _LogSourceGroup *group);
void cfg_add_dest(GlobalConfig *configuration, struct _LogDestGroup *group);
void cfg_add_filter(GlobalConfig *configuration, struct _LogFilterRule *rule);
void cfg_add_connection(GlobalConfig *configuration, struct _LogConnection *conn);
void cfg_add_template(GlobalConfig *cfg, struct _LogTemplate *template);
struct _LogTemplate *cfg_lookup_template(GlobalConfig *cfg, gchar *name);

void cfg_file_owner_set(GlobalConfig *self, gchar *owner);
void cfg_file_group_set(GlobalConfig *self, gchar *group);
void cfg_file_perm_set(GlobalConfig *self, gint perm);

void cfg_dir_owner_set(GlobalConfig *self, gchar *owner);
void cfg_dir_group_set(GlobalConfig *self, gchar *group);
void cfg_dir_perm_set(GlobalConfig *self, gint perm);
gint cfg_tz_convert_value(gchar *convert);
gint cfg_ts_format_value(gchar *format);
gboolean cfg_timezone_value(gchar *tz, glong *timezone);

GlobalConfig *cfg_new(gchar *fname);
void cfg_free(GlobalConfig *self);
gboolean cfg_init(GlobalConfig *cfg, PersistentConfig *persist);
gboolean cfg_deinit(GlobalConfig *cfg, PersistentConfig *persist);
GlobalConfig *cfg_reload_config(gchar *fname, GlobalConfig *cfg);

PersistentConfig *persist_config_new();
void persist_config_add(PersistentConfig *self, gchar *name, gpointer store, GDestroyNotify destroy);
gpointer persist_config_fetch(PersistentConfig *self, gchar *name);
void persist_config_free(PersistentConfig *persist);

#endif