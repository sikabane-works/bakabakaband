/*!
 * @file monster-drop-generator.h
 * @brief モンスター生成時にドロップ品を所持品として生成する処理
 */

#pragma once

class CreatureEntity;

/*!
 * @brief モンスターの一般ドロップ品 (drop_flags に基づくアイテム/金) を
 *        生成し、そのモンスターの所持品 (inventory) に追加する。
 * @param player プレイヤーへの参照 (生成基準・フロア取得用)
 * @param monster ドロップ品を持たせる対象モンスター
 * @details 従来は monster_death() 時に生成・床散布していた一般ドロップを、
 *          モンスター生成時に所持品として前生成する方式へ移行したもの。
 *          死亡時は drop_all_inventory() により所持品ごと床へ放出される。
 *          固定アーティファクト・クエスト品・死体ドロップ等の特殊処理は
 *          引き続き死亡時に行う。
 */
void generate_monster_drop_items(CreatureEntity &player, CreatureEntity &monster);

/*!
 * @brief SOLDIER / WARRIOR フラグを持つモンスターに初期装備の近接武器を持たせる。
 * @param monster 対象モンスター
 * @details 兵士・戦士は得物を携えているのが自然なため、生成時に種族レベル相応の
 *          近接武器を 1 本与えて利き手に装備させる。体構造的に武器を持てない
 *          個体 (四足・不定形・非実体等) と、既に利き手が埋まっている個体は対象外。
 *          **モンスターの装備武器は近接ダメージに加算される**
 *          (`calc_weapon_melee_damage`) ため、これは SOLDIER / WARRIOR 持ち
 *          モンスターへのバランス変更を伴う。武器の格は
 *          `decide_initial_weapon()` のレベル帯テーブルで調整すること。
 *          一般ドロップ (`generate_monster_drop_items`) より先に呼び、
 *          利き手を初期武器が確保できるようにする。
 */
void equip_armed_monster_initial_weapon(CreatureEntity &monster);
