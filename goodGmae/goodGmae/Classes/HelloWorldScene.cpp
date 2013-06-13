#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
	_tileMap = NULL;
	_background = NULL;
	_player = NULL;
}

HelloWorld::~HelloWorld()
{	
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {    
        CC_BREAK_IF(! CCLayer::init());

		this->setTileMap(CCTMXTiledMap::create("TileMap.tmx"));
		this->setBackground(_tileMap->layerNamed("Background"));
		this->addChild(_tileMap, -1);

		CCTMXObjectGroup *objects = _tileMap->objectGroupNamed("Objects");
		CCAssert(objects != NULL, "Objects' object group not found");
		CCDictionary *spawnPoint = objects->objectNamed("SpawnPoint");
		CCAssert(spawnPoint != NULL, "SpawnPoint object not found");
		int x = spawnPoint->valueForKey("x")->intValue();
		int y = spawnPoint->valueForKey("y")->intValue();

		this->setPlayer(CCSprite::create("Player.png"));
		_player->setPosition(ccp(x, y));
		this->addChild(_player);

		this->setViewpointCenter(_player->getPosition());

		this->setTouchEnabled(true);
        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::setViewpointCenter(CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
}

void HelloWorld::registerWithTouchDispatcher(void)
{
	 CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void HelloWorld::setPlayerPosition(CCPoint position)
{
	_player->setPosition(position);
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);

	CCPoint playerPos = _player->getPosition();
	CCPoint diff = ccpSub(touchLocation, playerPos);
	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			playerPos.x += _tileMap->getTileSize().width;
		} 
		else
		{
			playerPos.x -= _tileMap->getTileSize().width;
		}
	} 
	else
	{
		if (diff.y > 0)
		{
			playerPos.y += _tileMap->getTileSize().height;
		} 
		else
		{
			playerPos.y -= _tileMap->getTileSize().height;
		}		
	}
	
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
		playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
		playerPos.y >= 0 && playerPos.x >= 0)
	{
		setPlayerPosition(playerPos);
	}
	
	setViewpointCenter(_player->getPosition());
}






