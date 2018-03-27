/*
    This file is part of Kismet

    Kismet is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Kismet is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Kismet; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __DATASOURCE_RTL433_H__
#define __DATASOURCE_RTL433_H__

#include "config.h"

#include "kis_datasource.h"
#include "kis_net_microhttpd.h"

/* A light-weight HTTP based data source which receives JSON-encoded
 * sensor data from the rtl433 tool.
 *
 * Device records are received from the rtl433 capture tool via the kismet
 * rest interface, placed into a packet, and decoded by the phy
 */

class KisDatasourceRtl433;
typedef std::shared_ptr<KisDatasourceRtl433> SharedDatasourceRtl433;

class KisDatasourceRtl433 : public KisDatasource {
public:
    KisDatasourceRtl433(GlobalRegistry *in_globalreg, SharedDatasourceBuilder in_builder);
    virtual ~KisDatasourceRtl433();

    virtual bool dispatch_rx_packet(std::shared_ptr<KismetExternal::Command> c);
  
    virtual void handle_packet_rtl433device(uint32_t in_seqno, std::string in_content);

protected:
    virtual bool process_rtl_json(kis_packet *packet, std::string in_json);

    int pack_comp_rtl433, pack_comp_metablob;

};

class DatasourceRtl433Builder : public KisDatasourceBuilder {
public:
    DatasourceRtl433Builder(GlobalRegistry *in_globalreg, int in_id) :
        KisDatasourceBuilder(in_globalreg, in_id) {
        register_fields();
        reserve_fields(NULL);
        initialize();
    }

    DatasourceRtl433Builder(GlobalRegistry *in_globalreg, int in_id,
        SharedTrackerElement e) :
        KisDatasourceBuilder(in_globalreg, in_id, e) {

        register_fields();
        reserve_fields(e);
        initialize();
    }

    DatasourceRtl433Builder(GlobalRegistry *in_globalreg) :
        KisDatasourceBuilder(in_globalreg, 0) {

        register_fields();
        reserve_fields(NULL);
        initialize();
    }

    virtual ~DatasourceRtl433Builder() { }

    virtual SharedDatasource build_datasource(SharedDatasourceBuilder in_sh_this) {
        return SharedDatasourceRtl433(new KisDatasourceRtl433(globalreg, in_sh_this));
    }

    virtual void initialize() {
        set_source_type("rtl433");
        set_source_description("rtl_433 USB SDR");

        set_probe_capable(true);
        set_list_capable(true);
        set_local_capable(true);
        set_remote_capable(true);
        set_passive_capable(false);
        set_tune_capable(true);
    }
};

#endif

