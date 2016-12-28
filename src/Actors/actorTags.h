
/**
 * @file   actorTags.h
 * @brief  Actorのタグをまとめで管理する
 *
 * @author y.akira
 * @date   2016.12.28
 */

#pragma once


/**
 * @brief アクターのタグをまとめる
 */
enum ActorTags {
  NONE = -1,       ///< 設定されてない場合のタグ
  
  PLAYER,          ///< プレイヤー
  SPAWNER,         ///< スポナー
  BRICK,           ///< レンガ
}
