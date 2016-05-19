-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2+deb7u2
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Май 19 2016 г., 20:12
-- Версия сервера: 5.5.47
-- Версия PHP: 5.4.45-0+deb7u2

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- База данных: `Sensors`
--

-- --------------------------------------------------------

--
-- Структура таблицы `Sensors`
--

CREATE TABLE IF NOT EXISTS `Sensors` (
  `Id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Номер датчика',
  `IsUse` tinyint(1) NOT NULL,
  `Type` varchar(50) NOT NULL COMMENT 'Тип датчика',
  `Identifier` varchar(50) NOT NULL COMMENT 'Идентификатор датчика',
  `RefreshInterval` int(10) unsigned NOT NULL COMMENT 'Период обновления значений датчика (в секундах)',
  `Name` varchar(50) NOT NULL COMMENT 'Название датчика',
  `Description` varchar(250) NOT NULL COMMENT 'Описание',
  PRIMARY KEY (`Id`),
  KEY `Identifier` (`Identifier`),
  KEY `Type` (`Type`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='Датчики' AUTO_INCREMENT=14 ;

-- --------------------------------------------------------

--
-- Структура таблицы `Types`
--

CREATE TABLE IF NOT EXISTS `Types` (
  `Type` varchar(50) NOT NULL COMMENT 'Тип датчика',
  `ServerName` varchar(50) NOT NULL COMMENT 'Имя локального сервера для датчика',
  `Description` varchar(250) NOT NULL COMMENT 'Описание',
  PRIMARY KEY (`Type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Типы датчиков';

-- --------------------------------------------------------

--
-- Структура таблицы `Values`
--

CREATE TABLE IF NOT EXISTS `Values` (
  `DateTime` datetime NOT NULL COMMENT 'Дата и время значения',
  `SensorId` tinyint(3) unsigned NOT NULL COMMENT 'Номер датчика',
  `Value` float NOT NULL COMMENT 'Значение датчика'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Значения датчиков';

