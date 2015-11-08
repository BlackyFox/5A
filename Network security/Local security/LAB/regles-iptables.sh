#!/bin/bash
# Script de mise en place des regles du parefeu
# This script writes the netfilter rules
# tamplate by Rexy 
IPTABLES="/sbin/iptables"
# on passe en mode routeur
#echo 1 >  /proc/sys/net/ipv4/ip_forward

# Effacement des règles existantes
# Flush all existing rules
$IPTABLES -F
$IPTABLES -t nat -F
$IPTABLES -t mangle -F
$IPTABLES -F INPUT
$IPTABLES -F FORWARD
$IPTABLES -F OUTPUT

# Suppression des chaines utilisateurs sur les tables filter et nat
# Flush non default rules on filter and nat tables
$IPTABLES -X
$IPTABLES -t nat -X

# Stratégies par défaut
# Default policies
$IPTABLES -P INPUT ACCEPT
$IPTABLES -P FORWARD ACCEPT
$IPTABLES -P OUTPUT ACCEPT
$IPTABLES -t nat -P PREROUTING ACCEPT
$IPTABLES -t nat -P POSTROUTING ACCEPT
$IPTABLES -t nat -P OUTPUT ACCEPT
#entrez vos règles à partir d'ici :
