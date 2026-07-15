#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
using namespace KamataEngine;
class Player;

// 正面から倒せない敵。
// Enemy.h/.cppを複製してリネームしたクラス(継承ではなく別クラス)。
class ShieldEnemy {
public:
	static inline const float kWalkAnimationPeriod = 1.0f; // seconds per walk-rock cycle
	static inline const float kMaxRockAngle = 15.0f;       // degrees of walk-rock

	// 敵側の状態遷移(歩行 / デス演出)
	// NOTE: ガード成功時は専用のアニメーション状態を持たない。GuardEffectと
	// ノックバック要求だけを行い、見た目はそのまま歩行状態を継続する。
	enum class Behavior {
		kWalk,   // 歩行状態
		kDefeat, // デス演出
	};

	ShieldEnemy();

	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	AABB GetAABB();
	Vector3 GetWorldPosition();

	void Initialize(KamataEngine::Model* model, uint32_t textureHandleEnemy, KamataEngine::Camera* camera, Vector3& position);
	void update();
	void draw();
	void setMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// プレイヤーとの衝突応答。
	// NOTE: Enemyから複製した時点ではconst Player*だったが、ガード成功時に
	// player->RequestKnockback()を呼んでプレイヤー側のフラグを書き換える必要があるため、
	// 普通の(非const)ポインタに変更してある。constは「書き換え禁止」を意味するので、
	// 書き換えを行う以上constのままでは呼び出せない。
	void onCollision(Player* player);

	// ガードが成功した瞬間に一度だけtrueを返す。
	// GameScene側でこれを見てGuardEffectを生成したら、フラグは自動的に消費(false)される。
	bool ConsumeGuardEffectRequest();

	// 実際に撃破された(ガードできずデス演出に入った)瞬間に一度だけtrueを返す。
	// GameScene側でこれを見てHitEffect(通常の撃破エフェクト)を生成したら、
	// フラグは自動的に消費(false)される。
	bool ConsumeHitEffectRequest();

	// 既にデス演出に入っているか(GameScene側の重複判定・貫通に使う)
	bool IsDefeated() const { return behavior_ == Behavior::kDefeat; }
	// デス演出が最後まで再生し終えたか(GameScene側の削除タイミングに使う)
	bool IsDefeatAnimationFinished() const { return behavior_ == Behavior::kDefeat && defeatTimer_ >= kDefeatTime; }

private:
	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	struct CollisionMapInfo {
		bool isHitDown = false;
		bool isHitUp = false;
		bool isHitLeft = false;
		bool isHitRight = false;
		Vector3 velocityAfterCollision = {};
	};

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void CollisionMap(CollisionMapInfo& info);
	void CollisionMapTop(CollisionMapInfo& info);
	void CollisionMapBottom(CollisionMapInfo& info);
	void CollisionMapRight(CollisionMapInfo& info);
	void CollisionMapLeft(CollisionMapInfo& info);
	void CollisionDetected(const CollisionMapInfo& info);
	void ApplyCollisionResult(const CollisionMapInfo& info);

	void OnDefeated();
	void UpdateWalk();
	void UpdateDefeat();

	MapChipField* mapChipField_ = nullptr;
	WorldTransform worldTransform_;
	Model* enemyModel_ = nullptr;
	uint32_t textureHandleEnemy_ = 0;
	Camera* camera_ = nullptr;

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	bool onGround_ = false;
	// rotation_.y == +1.5fで右向き、-1.5fで左向き(Enemyの回転値と対応させてある)
	bool facingRight_ = false;
	
	float facingBaseAngleY_ = -1.5f;

	float walkTimer_ = 0.0f;

	Behavior behavior_ = Behavior::kWalk;

	// デス演出(正面以外から攻撃された場合)
	float defeatTimer_ = 0.0f;
	static inline const float kDefeatTime = 0.5f;
	static inline const float kDefeatSpinSpeed = 25.0f;

	// ガード成功時に生成するGuardEffect用の一度きりのフラグ
	bool guardEffectRequested_ = false; // GuardEffect生成待ちフラグ(GameScene側が消費する)
	bool hitEffectRequested_ = false;   // 撃破時のHitEffect生成待ちフラグ(GameScene側が消費する)

	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;
	static inline const float kWalkSpeed = 0.02f;
};