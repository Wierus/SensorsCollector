-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2+deb7u2
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Май 19 2016 г., 20:27
-- Версия сервера: 5.5.47
-- Версия PHP: 5.4.45-0+deb7u2

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- База данных: `Sensors`
--

--
-- Дамп данных таблицы `Types`
--

INSERT INTO `Types` (`Type`, `ServerName`, `Description`) VALUES
('DHT22', 'DHT22SensorServer', 'Датчик влажности DHT22'),
('DS18B20', 'DS18B20SensorServer', 'Датчик температуры DS18B20'),
('ElectricPower', 'ElectricPowerSensorServer', 'Счетчик электроэнергии'),
('SystemTemperature', 'SystemTemperatureSensorServer', 'Системный датчик температуры '),
('VoltagePresence', 'VoltagePresenceSensorServer', 'Датчик наличия напряжения');

