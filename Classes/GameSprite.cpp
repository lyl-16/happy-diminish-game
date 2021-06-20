/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "GameSprite.h"

Animation* GameAction::createAnimation(const string& plist, float delayPerUnit)
{
	if (plist.empty()) return nullptr;

	//加载Plist 到 SpriteFrameCache
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(plist);

	//获取字典
	string fullPath = FileUtils::getInstance()->fullPathForFilename(plist);
	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
	//遍历所有的帧
	ValueMap& frames = dict["frames"].asValueMap();
	auto animation = Animation::create();
	for (auto item : frames)
	{
		auto frame = cache->getSpriteFrameByName(item.first);
		animation->addSpriteFrame(frame);
	}

	//设置动画的播放间隔
	animation->setDelayPerUnit(delayPerUnit);
	animation->setRestoreOriginalFrame(true);
	return animation;
}
