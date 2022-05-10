#pragma once

/* アプリケーション構造体 */
typedef struct AppTag {
	/* 音声リソースオブジェクト */
	CriAtomExVoicePoolHn voice_pool;
	CriAtomExAcbHn acb_hn;
	CriAtomDbasId dbas_id;

	/* 3D音源 */
	CriAtomExPlayerHn player;			/* 再生プレーヤ */
	CriAtomEx3dSourceHn source;			/* 音源オブジェクト */
	CriAtomExVector source_pos;			/* 音源の位置 */
	CriAtomExVector source_velocity;	/* 音源の速度 */

	/* リスナー */
	CriAtomEx3dListenerHn listener;
	CriAtomExVector listener_pos;		/* リスナーの位置 */
	CriAtomExVector listener_front;		/* リスナーの向いている方向(前方ベクトル) */
	CriAtomExVector listener_top;		/* リスナーの向いている方向(上方ベクトル) */

	/* 3Dポジショニングのプレビューインデックス */
	//CriSint32 selecting_preview_index;
	//CriSint32 current_preview_index;
} AppObj;


