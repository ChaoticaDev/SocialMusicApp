-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.5.45-cll-lve


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema io_socialapp
--

CREATE DATABASE IF NOT EXISTS io_socialapp;
USE io_socialapp;

--
-- Definition of table `categories`
--

DROP TABLE IF EXISTS `categories`;
CREATE TABLE `categories` (
  `cat_name` varchar(45) NOT NULL,
  PRIMARY KEY (`cat_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `categories`
--

/*!40000 ALTER TABLE `categories` DISABLE KEYS */;
INSERT INTO `categories` (`cat_name`) VALUES 
 ('Business'),
 ('Electric'),
 ('Hip-Hop'),
 ('Politics'),
 ('Pop'),
 ('Popular'),
 ('Trending');
/*!40000 ALTER TABLE `categories` ENABLE KEYS */;


--
-- Definition of table `favorites`
--

DROP TABLE IF EXISTS `favorites`;
CREATE TABLE `favorites` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL,
  `track_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `favorites`
--

/*!40000 ALTER TABLE `favorites` DISABLE KEYS */;
/*!40000 ALTER TABLE `favorites` ENABLE KEYS */;


--
-- Definition of table `likes`
--

DROP TABLE IF EXISTS `likes`;
CREATE TABLE `likes` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `snippet_id` int(10) unsigned NOT NULL,
  `like_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `likes`
--

/*!40000 ALTER TABLE `likes` DISABLE KEYS */;
INSERT INTO `likes` (`ID`,`uid`,`snippet_id`,`like_timestamp`) VALUES 
 (3,1,1,'2016-03-15 20:48:41'),
 (4,3,1,'2016-04-03 09:56:33');
/*!40000 ALTER TABLE `likes` ENABLE KEYS */;


--
-- Definition of table `relations`
--

DROP TABLE IF EXISTS `relations`;
CREATE TABLE `relations` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL,
  `follower_id` int(10) unsigned NOT NULL,
  `friendship_established_on` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `relations`
--

/*!40000 ALTER TABLE `relations` DISABLE KEYS */;
INSERT INTO `relations` (`ID`,`uid`,`follower_id`,`friendship_established_on`) VALUES 
 (1,1,2,'2016-03-15 21:09:34'),
 (2,1,3,'2016-03-15 21:09:34');
/*!40000 ALTER TABLE `relations` ENABLE KEYS */;


--
-- Definition of table `snippets`
--

DROP TABLE IF EXISTS `snippets`;
CREATE TABLE `snippets` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL,
  `title` varchar(45) NOT NULL,
  `description` varchar(150) NOT NULL,
  `filename` varchar(255) NOT NULL,
  `album` varchar(255) NOT NULL,
  `cover` varchar(255) NOT NULL DEFAULT 'https://ubersnip.com/ubersnip.jpg',
  `genre` varchar(45) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `snippets`
--

/*!40000 ALTER TABLE `snippets` DISABLE KEYS */;
INSERT INTO `snippets` (`ID`,`uid`,`title`,`description`,`filename`,`album`,`cover`,`genre`) VALUES 
 (1,2,'Heartbeat','Jules M. Heartbeat','','UberSnip Featured','http://api.ubersnip.com/Assets/julesmavatar.jpg','Pop'),
 (3,1,'Don\'t wanna try','','','UberSnip Featured','http://api.ubersnip.com/Assets/kbustavatar.jpg','Electric');
/*!40000 ALTER TABLE `snippets` ENABLE KEYS */;


--
-- Definition of table `user_token`
--

DROP TABLE IF EXISTS `user_token`;
CREATE TABLE `user_token` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL,
  `token_string` varchar(255) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=102 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `user_token`
--

/*!40000 ALTER TABLE `user_token` DISABLE KEYS */;
INSERT INTO `user_token` (`ID`,`uid`,`token_string`) VALUES 
 (1,1,'56ed5883e6f3b56ed5883e6f7556ed5883e6fac'),
 (2,1,'56ed5a1808de856ed5a1808e2156ed5a1808e59'),
 (3,1,'56ed5ae10dd9c56ed5ae10ddd556ed5ae10de0d'),
 (4,1,'56ed7df2c451f56ed7df2c455b56ed7df2c4566'),
 (5,1,'56ed7e7a2866c56ed7e7a286a556ed7e7a286dd'),
 (6,1,'56ed82cc949c756ed82cc94a0456ed82cc94a41'),
 (7,1,'56ed85441273256ed85441277156ed8544127ad'),
 (8,1,'56ed85d456f0956ed85d456f4f56ed85d456f89'),
 (9,1,'56ed868ca57bd56ed868ca57fb56ed868ca5837'),
 (10,1,'56ed86cb75f3256ed86cb75f5556ed86cb75fad'),
 (11,1,'56ed87abd38a356ed87abd38dd56ed87abd3915'),
 (12,1,'56ed888fabfb156ed888fabfed56ed888fac027'),
 (13,1,'56ed8dcd1e7c556ed8dcd1e80456ed8dcd1e840'),
 (14,1,'56ed9172bed8556ed9172bedbe56ed9172bedf6'),
 (15,1,'56ed925fb5aee56ed925fb5b2a56ed925fb5b65'),
 (16,1,'56ed925fe6d0756ed925fe6d4556ed925fe6d81'),
 (17,1,'56ed94042917d56ed9404291b656ed9404291ed'),
 (18,1,'56ed959aa9e2f56ed959aa9e7056ed959aa9eaf'),
 (19,1,'56ed96366bad756ed96366bb1556ed96366bb5f'),
 (20,1,'56ed96aebdc6f56ed96aebdca956ed96aebdce0'),
 (21,1,'56ed97a394c1856ed97a394c5656ed97a394c94'),
 (22,1,'56ee3cf0d0cb056ee3cf0d0cea56ee3cf0d0d22'),
 (23,1,'56ee3cf115f0656ee3cf115f4056ee3cf115f77'),
 (24,1,'56ee3e70eae9756ee3e70eaed056ee3e70eaf08'),
 (25,1,'56eea1993682e56eea1993686656eea1993689d'),
 (26,1,'56eea24f198bc56eea24f198f956eea24f19934'),
 (27,1,'56eea24f5175756eea24f5178f56eea24f517c7'),
 (28,1,'56eea2ce010b856eea2ce010f856eea2ce01139'),
 (29,1,'56eea2ffc66e156eea2ffc671f56eea2ffc6758'),
 (30,1,'56eea372812c756eea3728130156eea37281345'),
 (31,1,'56eea3c87271b56eea3c87275456eea3c87278b'),
 (32,1,'56eea49e5b85d56eea49e5b89556eea49e5b8cd'),
 (33,1,'56eea594e0dab56eea594e0de956eea594e0e29'),
 (34,1,'56eea6a7c0fa156eea6a7c0fdf56eea6a7c101b'),
 (35,1,'56eea6a7f382456eea6a7f386956eea6a7f38a5'),
 (36,1,'56eea7fbd85e956eea7fbd862356eea7fbd865b'),
 (37,1,'56eea8789361156eea8789364c56eea87893686'),
 (38,1,'56eea878c377256eea878c37ab56eea878c37e3'),
 (39,1,'56eeb10b6ae5d56eeb10b6ae9756eeb10b6aecf'),
 (40,1,'56eed7253f54856eed7253f58256eed7253f5ba'),
 (41,1,'56eed814cc57e56eed814cc5bc56eed814cc5f9'),
 (42,1,'56eed9070d23c56eed9070d27b56eed9070d2b8'),
 (43,1,'56eed96e66c5c56eed96e66c9556eed96e66ccd'),
 (44,1,'56eed9b11d13b56eed9b11d17856eed9b11d1a4'),
 (45,1,'56eeda35219b556eeda35219fc56eeda3521a38'),
 (46,1,'56eeda6d8a72856eeda6d8a76356eeda6d8a781'),
 (47,1,'56eeda73d19a456eeda73d19e256eeda73d1a1e'),
 (48,1,'56eeda75e0e2e56eeda75e0e3f56eeda75e0e7b'),
 (49,1,'56eedab47c39c56eedab47c3d756eedab47c40f'),
 (50,1,'56eedaf2a6d7a56eedaf2a6db856eedaf2a6df9'),
 (51,3,'56ef5d35848c256ef5d35848ff56ef5d358493c'),
 (52,3,'56ef5d3608f7656ef5d3608faf56ef5d3608fe7'),
 (53,3,'56ef90751937256ef9075193ab56ef9075193e3'),
 (54,3,'56ef98abd821056ef98abd824956ef98abd8282'),
 (55,1,'56f0071a8191456f0071a8194e56f0071a81986'),
 (56,1,'56f008307548056f00830754ba56f00830754f1'),
 (57,1,'56f008beb433056f008beb436956f008beb4395'),
 (58,1,'56f00b79a13b056f00b79a13ed56f00b79a1429'),
 (59,1,'56f00b9ab5b4b56f00b9ab5b8456f00b9ab5bbb'),
 (60,1,'56f00c4fd604b56f00c4fd608956f00c4fd60c4'),
 (61,1,'56f0307c77e9056f0307c77ed056f0307c77f08'),
 (62,1,'56f0319ded70856f0319ded74356f0319ded77b'),
 (63,1,'56f03274b76a756f03274b76e656f03274b7723'),
 (64,1,'56f03328383ee56f033283843556f0332838471'),
 (65,1,'56f1e4bd17e1f56f1e4bd17e5856f1e4bd17e90'),
 (66,1,'56f2ea7a27bfc56f2ea7a27c3956f2ea7a27c76'),
 (67,3,'57003de5b3af957003de5b3b3b57003de5b3b75'),
 (68,3,'57003f7068c3e57003f7068c6457003f7068ca6'),
 (69,3,'5700417a55d055700417a55d465700417a55d83'),
 (70,3,'57004562c565957004562c573857004562c5778'),
 (71,3,'57004935918b757004935918f75700493591934'),
 (72,3,'57004963e78b957004963e78f657004963e7931'),
 (73,3,'57005d779888757005d77988c257005d77988fc'),
 (74,3,'57005e712112957005e712116757005e71211a1'),
 (75,3,'57005eea8c26b57005eea8c2a457005eea8c2cb'),
 (76,3,'57005fb89610057005fb89618a57005fb8961c9'),
 (77,3,'57006023e3c7957006023e3cb357006023e3cea'),
 (78,3,'57006046619d95700604661a145700604661a4c'),
 (79,3,'5700608f488c55700608f489075700608f48940'),
 (80,3,'5700609d341c35700609d341fd5700609d34235'),
 (81,3,'57006110dd43257006110dd47257006110dd4b1'),
 (82,3,'570061233f8d5570061233f910570061233f94b'),
 (83,3,'570061de1f958570061de1f995570061de1f9d1'),
 (84,3,'570061fc3c1a0570061fc3c1d9570061fc3c211'),
 (85,3,'570068aa3f943570068aa3f97c570068aa3f9b3'),
 (86,3,'570135c39ae42570135c39ae7f570135c39aeb9'),
 (87,3,'570136acaf728570136acaf761570136acaf798'),
 (88,3,'5701376597159570137659719257013765971d1'),
 (89,3,'570137e0748f7570137e074930570137e07496a'),
 (90,3,'57013bf15870457013bf15874457013bf158781'),
 (91,3,'570140905dec9570140905df05570140905df3f'),
 (92,3,'570140cf2a02f570140cf2a06a570140cf2a084'),
 (93,3,'570141636dd93570141636ddd1570141636de18'),
 (94,3,'5701460ba24c55701460ba25025701460ba2544'),
 (95,3,'570146a7c43cd570146a7c4407570146a7c4440'),
 (96,3,'570147303159957014730315ea5701473031622'),
 (97,3,'5701479ba81ed5701479ba822b5701479ba8268'),
 (98,3,'57014b673053557014b673057157014b67305ab'),
 (99,3,'570156c9c23e7570156c9c2426570156c9c245f'),
 (100,3,'570159785849157015978584d0570159785850c'),
 (101,3,'57015bd9df77c57015bd9df7b557015bd9df7f8');
/*!40000 ALTER TABLE `user_token` ENABLE KEYS */;


--
-- Definition of table `users`
--

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `uid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(25) NOT NULL,
  `password` varchar(45) NOT NULL,
  `avatar` varchar(255) NOT NULL,
  `cover` varchar(255) NOT NULL,
  `bio` varchar(255) NOT NULL,
  `location` varchar(255) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` (`uid`,`username`,`password`,`avatar`,`cover`,`bio`,`location`) VALUES 
 (1,'KBust','KBust','http://api.ubersnip.com/Assets/kbustavatar.jpg','http://api.ubersnip.com/Assets/kbustavatar.jp','K Bust','Canada'),
 (2,'JulesM','JulesM','http://api.ubersnip.com/Assets/julesm.jpg','http://api.ubersnip.com/Assets/julesmavatar.jpg','Jules M.','United States'),
 (3,'montraydavis','montraydavis','https://ubersnip.com/ubersnip.jpg','https://ubersnip.com/ubersnip.jpg','Montray Davis','United States');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;


--
-- Definition of table `views`
--

DROP TABLE IF EXISTS `views`;
CREATE TABLE `views` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `snippet_id` int(10) unsigned NOT NULL,
  `view_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `views`
--

/*!40000 ALTER TABLE `views` DISABLE KEYS */;
INSERT INTO `views` (`ID`,`uid`,`snippet_id`,`view_timestamp`) VALUES 
 (1,1,1,'2016-03-15 20:47:01'),
 (2,1,1,'2016-03-15 20:47:33'),
 (3,1,1,'2016-03-15 20:51:57');
/*!40000 ALTER TABLE `views` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
