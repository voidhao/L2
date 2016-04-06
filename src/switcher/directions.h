#ifndef _LONLIFE_SWITCH_DIRECTIONS_H_
#define _LONLIFE_SWITCH_DIRECTIONS_H_

namespace switcher{

    struct DirClient{
        enum {
            value = 1,
        };
    };
    struct DirSwitcher{
        enum {
            value = 2,
        };
    };
    struct DirOut{
        enum{
            value = 3,
        };
    };

    template <typename FROM, typename TO>
    struct DataFlow{
        using  From = FROM;
        using  To   = TO;
    };

    using client_to_nat = DataFlow<DirClient, DirOut>;
    using nat_to_client = DataFlow <DirOut, DirClient>;

    const int client_way = 1;
    const int switch_way = 2;
    const int out_way	 = 3;
} // namespace switcher
#endif //_LONLIFE_SWITCH_DIRECTIONS_H_
