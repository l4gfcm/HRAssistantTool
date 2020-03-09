CREATE DATABASE  IF NOT EXISTS `hrat` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */;
USE `hrat`;
-- MariaDB dump 10.17  Distrib 10.4.12-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: hrat
-- ------------------------------------------------------
-- Server version	10.4.12-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `history`
--

DROP TABLE IF EXISTS `history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `history` (
  `id_history` int(11) NOT NULL AUTO_INCREMENT,
  `fid_worker` int(11) NOT NULL,
  `comment` varchar(128) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `fid_step` int(11) NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY (`id_history`,`fid_worker`),
  KEY `fk_history_step` (`fid_step`),
  KEY `fk_history_user` (`fid_worker`),
  CONSTRAINT `fk_history_step` FOREIGN KEY (`fid_step`) REFERENCES `workflow` (`id_step`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_history_user` FOREIGN KEY (`fid_worker`) REFERENCES `workers` (`id_worker`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=86 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `history`
--

LOCK TABLES `history` WRITE;
/*!40000 ALTER TABLE `history` DISABLE KEYS */;
/*!40000 ALTER TABLE `history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vacancies`
--

DROP TABLE IF EXISTS `vacancies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vacancies` (
  `id_vacancy` int(11) NOT NULL AUTO_INCREMENT,
  `vname` varchar(45) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id_vacancy`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vacancies`
--

LOCK TABLES `vacancies` WRITE;
/*!40000 ALTER TABLE `vacancies` DISABLE KEYS */;
INSERT INTO `vacancies` VALUES (0,'Developer'),(1,'HR');
/*!40000 ALTER TABLE `vacancies` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workers`
--

DROP TABLE IF EXISTS `workers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workers` (
  `id_worker` int(11) NOT NULL AUTO_INCREMENT,
  `fname` varchar(45) COLLATE utf8mb4_unicode_ci NOT NULL,
  `lname` varchar(45) COLLATE utf8mb4_unicode_ci NOT NULL,
  `mphone` varchar(45) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `next_date` datetime DEFAULT NULL,
  `fd_vacancy` int(11) NOT NULL,
  `fd_state` int(11) NOT NULL,
  PRIMARY KEY (`id_worker`),
  KEY `fk_workers_state` (`fd_state`),
  KEY `fk_workers_vacancy` (`fd_vacancy`),
  CONSTRAINT `fk_workers_state` FOREIGN KEY (`fd_state`) REFERENCES `workflow` (`id_step`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workers_vacancy` FOREIGN KEY (`fd_vacancy`) REFERENCES `vacancies` (`id_vacancy`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workers`
--

LOCK TABLES `workers` WRITE;
/*!40000 ALTER TABLE `workers` DISABLE KEYS */;
/*!40000 ALTER TABLE `workers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workflow`
--

DROP TABLE IF EXISTS `workflow`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workflow` (
  `id_step` int(11) NOT NULL AUTO_INCREMENT,
  `pid_step` int(11) DEFAULT NULL,
  `sname` varchar(45) COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`id_step`),
  KEY `fk_workflow_step` (`pid_step`),
  CONSTRAINT `fk_workflow_step` FOREIGN KEY (`pid_step`) REFERENCES `workflow` (`id_step`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workflow`
--

LOCK TABLES `workflow` WRITE;
/*!40000 ALTER TABLE `workflow` DISABLE KEYS */;
INSERT INTO `workflow` VALUES (0,NULL,'Start'),(1,0,'Next'),(2,1,'Finish'),(3,0,'Cancel'),(4,0,'Rejected'),(8,1,'Interview'),(9,8,'Meeting'),(10,9,'Offer'),(11,8,'Rejected'),(12,9,'Rejected'),(13,10,'Rejected'),(14,10,'Hired');
/*!40000 ALTER TABLE `workflow` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'hrat'
--

--
-- Dumping routines for database 'hrat'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-03-09 11:53:44
