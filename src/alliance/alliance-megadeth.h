#include "alliance.h"

class AllianceMegadeth : public Alliance {
public:
    using Alliance::Alliance;
    AllianceMegadeth() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラsグ
    int calcImpressionPoint(PlayerType *creature_ptr) const override;
    bool isAnnihilated() override;
    virtual ~AllianceMegadeth() = default;
};
