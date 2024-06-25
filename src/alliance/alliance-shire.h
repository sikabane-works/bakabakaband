#include "alliance.h"

class AllianceTheShire : public Alliance {
public:
    using Alliance::Alliance;
    AllianceTheShire() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImpressionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceTheShire() = default;
};
