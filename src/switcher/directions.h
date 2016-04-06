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
} // namespace switcher
#endif //_LONLIFE_SWITCH_DIRECTIONS_H_
