/*
 * Copyright (C) 2006-2009 Alex Lasnier <alex@fepg.org>
 *
 * This file is part of ATSC EPG
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ATSC_STRUCTS_H
#define __ATSC_STRUCTS_H

#include <string>

#include <vdr/channels.h>

#include "tools.h"


//////////////////////////////////////////////////////////////////////////////


struct Table 
{
  Table(void);
  Table(u16 p, u8 t, u16 tt);
  Table(const Table& arg);
  const Table& operator= (const Table& arg);
  
	u16 pid;
	u8  tid;
	u16 table_type;
};


//////////////////////////////////////////////////////////////////////////////


struct Event
{
  Event(void);
 ~Event(void); 
  Event(const Event& arg);
  
  const Event& operator= (const Event& arg);
  
  void SetTitleText(const char* text);
  const char* TitleText(void) const { return title_text; }
  
  u16 event_id;
  u32 start_time;
  u32 length_in_seconds;
  u8 version_number;
  u8 table_id;
  u8 ETM_location;
  
private:
  char* title_text;  
};


//////////////////////////////////////////////////////////////////////////////


class AtscChannel
{
public:
  AtscChannel(void);

  u16 MajorNumber(void) const { return majorChannelNumber; }
  u16 MinorNumber(void) const { return minorChannelNumber; }
  int Sid(void) const { return channel.Sid(); }
  const char* LongName(void) const  { return channel.Name(); }
  const char* ShortName(void) const { return channel.ShortName(); }
  cChannel* VDRChannel(void) { return &channel; }
    
  void SetMajorNumber(u16 n) { majorChannelNumber = n; }
  void SetMinorNumber(u16 n) { minorChannelNumber = n; }
  void SetId(int Tid, int Sid) { channel.SetId(0, Tid, Sid); };
  void SetShortName(const char* n) { channel.SetName(n, n, ""); }
  void SetLongName(const char* n) { channel.SetName(n, channel.ShortName(), ""); }
  void SetPids(int Vpid, int Ppid, int Vtype, int *Dpids, char DLangs[][MAXLANGCODE2]);
  void SetNumber(int Number) { channel.SetNumber(Number); }
  
private:
  cChannel channel;
  
	u16 majorChannelNumber;
	u16 minorChannelNumber;
};

/*
struct Channel
{ 
  Channel(void);
 ~Channel(void);
  Channel(const Channel& arg);

  const Channel& operator= (const Channel& arg);
  
  void SetShortName(const char* shortName);
  void SetLongName(const char* longName);
  
  const char* ShortName(void) const { return short_name; }
  const char* LongName(void)  const { return long_name;  }
  
  u16 transport_stream_id;
  u16 source_id;
	
	u16 vPID;
	u16 PCR_PID;
	u16 majorChannelNumber;
	u16 minorChannelNumber;
	
	char aLang[4];
		
private:
  char* short_name;
  char* long_name;
};
*/

//////////////////////////////////////////////////////////////////////////////


struct Stream
{
  Stream(void) { stream_type = 0; elementary_PID=0; ISO_639_language_code[0]=0; }
	
	u8  stream_type;
  u16 elementary_PID;

	char ISO_639_language_code[4];
};

//////////////////////////////////////////////////////////////////////////////


#endif //__ATSC_STRUCTS_H
