/*
 * Copyright (C) 2006-2010 Alex Lasnier <alex@fepg.org>
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

#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "tables.h"


//////////////////////////////////////////////////////////////////////////////


Table::Table(void) 
{ 
  pid = 0; 
  tid = 0; 
  table_type = 0;
  number_bytes = 0;
}


//////////////////////////////////////////////////////////////////////////////


Event::Event(void)
{
  event_id          = 0;
  start_time        = 0;
  length_in_seconds = 0;
  version_number    = 0;
  table_id          = 0;
  ETM_location      = 0;
  title_text        = NULL;
}


//----------------------------------------------------------------------------

Event::~Event(void)
{
  free(title_text);
}


//----------------------------------------------------------------------------

Event::Event(const Event& arg)
{
  event_id          = arg.event_id;
  start_time        = arg.start_time;
  length_in_seconds = arg.length_in_seconds;
  version_number    = arg.version_number;
  table_id          = arg.table_id;
  ETM_location      = arg.ETM_location;
  title_text        = strdup(arg.title_text);
}


//----------------------------------------------------------------------------
  
const Event& Event::operator= (const Event& arg)
{
  event_id          = arg.event_id;
  start_time        = arg.start_time;
  length_in_seconds = arg.length_in_seconds;
  version_number    = arg.version_number;
  table_id          = arg.table_id;
  ETM_location      = arg.ETM_location;
  free(title_text);
  title_text        = strdup(arg.title_text);
  return *this;
}


//----------------------------------------------------------------------------

void Event::SetTitleText(const char* text)
{
  free(title_text);
  title_text = strdup(text);
}


//////////////////////////////////////////////////////////////////////////////


AtscChannel::AtscChannel(void)
{
  majorChannelNumber = 0;
  minorChannelNumber = 0;
  sid = 0;
  hasEit = true;
}


//----------------------------------------------------------------------------

void AtscChannel::SetPids(int Vpid, int Ppid, int Vtype, int *Dpids, char DLangs[][MAXLANGCODE2])
{
  int Apids[1] = { 0 };
  int Spids[1] = { 0 };
  char ALangs[1][MAXLANGCODE2] = { "" };
  char SLangs[1][MAXLANGCODE2] = { "" };
#if VDRVERSNUM < 10715  
  channel.SetPids(Vpid, Ppid, Vtype, Apids, ALangs, Dpids, DLangs, Spids, SLangs, 0);
#else
  int Atypes[1] = { 0 };
  int Dtypes[MAXDPIDS + 1] = { 0 };
  for (int i=0; i<MAXDPIDS && Dpids[i]; i++)
    Dtypes[i] = 0x6A; // SI::AC3DescriptorTag

  channel.SetPids(Vpid, Ppid, Vtype, Apids, Atypes, ALangs, Dpids, Dtypes, DLangs, Spids, SLangs, 0);
#endif
}


//////////////////////////////////////////////////////////////////////////////


SidTranslator::SidTranslator(void)
{
  map = NULL;
  size = 0;
}


//----------------------------------------------------------------------------

SidTranslator::~SidTranslator()
{
  Clear();
}


//----------------------------------------------------------------------------


void SidTranslator::Update(VCT* vct)
{
  Clear();

  size = vct->NumberOfChannels();
  map = new SidPair[size];
  
  for (int i=0; i<size; i++)
  {
    const AtscChannel* ch = vct->GetChannel(i);
    map[i].vctSid = ch->Sid();
    map[i].pmtSid = ch->ProgramNumber();
  }
}


//----------------------------------------------------------------------------


uint16_t SidTranslator::GetPmtSid(uint16_t sid) const
{
  if (!map)
    return 0;
  
  for (int i=0; i<size; i++)
    if (map[i].vctSid == sid)
      return map[i].pmtSid;
      
  return 0;
}


//----------------------------------------------------------------------------


void SidTranslator::Clear(void)
{
  size = 0;
  delete[] map;
  map = NULL;
}


//////////////////////////////////////////////////////////////////////////////


