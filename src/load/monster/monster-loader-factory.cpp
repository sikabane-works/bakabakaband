/*!
 * @brief モンスター情報をセーブデータから読み込むクラスを選択するファクトリクラス
 * @date 2021/10/16
 * @author Hourier
 */

#include "load/monster/monster-loader-factory.h"
#include "load/load-util.h"
#include "load/monster/monster-loader-base.h"
#include "load/monster/monster-loader-version-types.h"
#include "load/old/monster-loader-savefile50.h"

/*!
 * @brief アイテム読み込みクラスを返却する.
 * @return アイテム読み込みクラスへの参照ポインタ.
 * @details MonsterLoaderBaseは純粋仮想関数を含むので参照を返す必要がある.
 * (値を返す設計はコンパイルエラー)
 */
std::shared_ptr<MonsterLoaderBase> MonsterLoaderFactory::create_loader()
{
    auto version = get_version();
    switch (version) {
    case MonsterLoaderVersionType::LOAD50:
    case MonsterLoaderVersionType::LOAD51:
        return std::make_shared<MonsterLoader50>();
        // dummy yet.
    default:
        THROW_EXCEPTION(std::logic_error, "Invalid loader version was specified!");
    }
}

/*!
 * @brief MonsterLoaderのバージョン切り替え.
 * @return セーブファイルバージョン群の中で互換性のある最古のバージョン.
 * @details (備忘録)例えばバージョン15で更に変更された場合、以下のように書き換えること.
 *
 * if (loading_savefile_version_is_older_than(15)) {
 *   return MonsterLoaderVersionType::LOAD11;
 * } else if (loading_savefile_version_is_older_than(11)) {
 *   return MonsterLoaderVersionType::LOAD10;
 * } else {
 *   return MonsterLoaderVersionType::LOAD15;
 * }
 */
MonsterLoaderVersionType MonsterLoaderFactory::get_version()
{
    if (loading_savefile_version_is_older_than(15)) {
        return MonsterLoaderVersionType::LOAD50;
    } else {
        return MonsterLoaderVersionType::LOAD51;
    }
}
