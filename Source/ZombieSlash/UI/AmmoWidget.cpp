// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AmmoWidget.h"
#include <iomanip>
#include <sstream>

FText UAmmoWidget::IntToZeroPaddedString(int32 Number, int32 TotalLength)
{
    std::stringstream Stream;
    Stream << std::setw(TotalLength) << std::setfill('0') << Number;
    std::string Result = Stream.str();
    return FText::FromString(Result.c_str());
}
