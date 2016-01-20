set names utf8; 

CREATE TABLE  `tb_users` (
  `UserID` int(11) NOT NULL AUTO_INCREMENT,					-- 用户编号
  `Name` varchar(20) NOT NULL,								-- 用户名称（代码中的UID通常是Name）
  `Password` varchar(32) NOT NULL,							-- 用户密码
  `status` int(2) NOT NULL DEFAULT '0',						-- 用户状态（待定）
  `ip` varchar(30) NOT NULL,								-- 注册地IP
  PRIMARY KEY (`UserID`),
  KEY `Name` (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE  `tb_reply` (
  `ReplyID` int(11) NOT NULL AUTO_INCREMENT,				-- 回复编号
  `Name` varchar(20) NOT NULL,								-- 回复人ID
  `Time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',	-- 回复时间
  `content` text NOT NULL,									-- 回复正文
  `TopicID` int(11) NOT NULL,								-- 所回主题
  `status` int(2) NOT NULL DEFAULT '0',						-- 回复状态（待定）
  `ip` varchar(30) NOT NULL,								-- 回复人IP
  PRIMARY KEY (`ReplyID`),
  KEY `Name` (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE  `tb_topic` (
  `TopicID` int(11) NOT NULL AUTO_INCREMENT,				-- 话题编号
  `Name` varchar(20) NOT NULL,								-- 发起人ID
  `Lastreply_Name` varchar(20) NOT NULL,					-- 最后回复
  `Lastreply_Time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',	-- 最后回复时间
  `Title` varchar(40) NOT NULL,								-- 话题标题
  `preview_content` text NOT NULL,							-- 一楼预览
  `ReplyCount` int(11) NOT NULL DEFAULT '1',				-- 回复数量
  `status` int(2) NOT NULL DEFAULT '0',						-- 话题状态（待定）
  `top_level` int(2) NOT NULL DEFAULT '0',					-- 置顶状态
  `good_level` int(2) NOT NULL DEFAULT '0',					-- 精品状态
  PRIMARY KEY (`TopicID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;