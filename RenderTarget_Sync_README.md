# Runner - RenderTarget 메시 동기화 시스템

이 시스템은 Runner 클래스의 메시가 변경될 때 RenderTarget의 메시도 **무조건 자동으로 동일하게 변경**되도록 하는 기능입니다.

## 주요 기능

### 1. **무조건 자동 메시 동기화**
- Runner의 메시가 변경되면 RenderTarget의 해당 메시도 **무조건 자동으로 동기화**됩니다.
- 모든 메시 컴포넌트 (Hair, UpperBody, Arms, LowerBody 등)가 지원됩니다.
- **동기화를 비활성화할 수 없습니다** - 항상 동기화됩니다.

### 2. **즉시 동기화**
- 메시 변경 시 즉시 동기화됩니다.
- 추가로 Tick에서 주기적으로도 동기화를 체크하여 누락을 방지합니다.

### 3. **수동 동기화**
- 언제든지 수동으로 전체 동기화를 실행할 수 있습니다.
- 특정 메시 컴포넌트만 선택적으로 동기화할 수 있습니다.

## 사용법

### 1. RenderTarget 설정
```cpp
// Runner 클래스에서 RenderTarget 참조 설정
ACNox_Runner* Runner = GetPlayerPawn<ACNox_Runner>();
ACNox_RenderTarget* RenderTarget = GetWorld()->SpawnActor<ACNox_RenderTarget>();

// RenderTarget 연결 (연결 즉시 동기화됨)
Runner->SetRenderTarget(RenderTarget);
```

### 2. 메시 변경 (무조건 동기화됨)
```cpp
// 헤어 메시 변경 (무조건 RenderTarget에 동기화됨)
USkeletalMesh* NewHairMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Path/To/HairMesh"));
Runner->SetHairMesh(NewHairMesh);

// 상의 메시 변경 (무조건 RenderTarget에 동기화됨)
USkeletalMesh* NewUpperBodyMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Path/To/UpperBodyMesh"));
Runner->SetUpperBodyMesh(NewUpperBodyMesh);

// 팔 메시 변경 (무조건 RenderTarget에 동기화됨)
USkeletalMesh* NewArmsMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Path/To/ArmsMesh"));
Runner->SetArmsMesh(NewArmsMesh);
```

### 3. 수동 동기화
```cpp
// 전체 메시 동기화
Runner->SyncMeshToRenderTarget();

// 특정 메시 컴포넌트 동기화
SyncMeshComponent(Hair, RenderTarget->GetHairMesh());
SyncMeshComponent(UpperBody, RenderTarget->GetUpperBodyMesh());
SyncMeshComponent(Outer, RenderTarget->GetOuterMesh());
SyncMeshComponent(Arms, RenderTarget->GetArmsMesh());
SyncMeshComponent(LowerBody, RenderTarget->GetLowerBodyMesh());
SyncMeshComponent(Foot, RenderTarget->GetFootMesh());
SyncMeshComponent(ChestRig, RenderTarget->GetChestRigMesh());
SyncMeshComponent(Backpack, RenderTarget->GetBackpackMesh());
SyncMeshComponent(FPSArms, RenderTarget->GetFPSArmsMesh());
SyncMeshComponent(WeaponSlot1, RenderTarget->GetWeaponSlot1Mesh());
SyncMeshComponent(WeaponSlot2, RenderTarget->GetWeaponSlot2Mesh());
```

### 4. 테스트
```cpp
// RenderTarget 동기화 테스트
Runner->TestRenderTargetSync();
```

## 지원되는 메시 컴포넌트

- **메인 메시**: GetMesh()
- **헤어**: Hair
- **상의**: UpperBody
- **외투**: Outer
- **팔**: Arms
- **하의**: LowerBody
- **신발**: Foot
- **가슴장비**: ChestRig
- **백팩**: Backpack
- **FPS 팔**: FPSArms
- **무기 슬롯1**: WeaponSlot1
- **무기 슬롯2**: WeaponSlot2

## 동기화되는 요소

1. **스켈레탈 메시**: 메시 자체의 변경
2. **머티리얼**: 각 메시에 적용된 머티리얼
3. **가시성**: 메시의 표시/숨김 상태

## 동기화 방식

### 1. **즉시 동기화**
- `SetHairMesh()`, `SetUpperBodyMesh()` 등의 함수 호출 시 즉시 동기화
- `OnMeshChanged()` 함수에서 변경된 컴포넌트만 즉시 동기화

### 2. **주기적 동기화**
- Tick에서 0.1초마다 메시 변경 여부를 체크
- 변경이 감지되면 전체 동기화 실행

## 주의사항

1. **RenderTarget 설정**: 동기화를 사용하기 전에 반드시 `SetRenderTarget()`을 호출하여 RenderTarget을 설정해야 합니다.

2. **무조건 동기화**: 이 시스템은 **동기화를 비활성화할 수 없습니다**. Runner의 메시가 변경되면 항상 RenderTarget에 동기화됩니다.

3. **메시 변경 함수 사용**: 직접 `SetSkeletalMesh()`를 호출하는 대신 제공된 함수들(`SetHairMesh()`, `SetUpperBodyMesh()` 등)을 사용하면 자동으로 동기화됩니다.

4. **에러 처리**: RenderTarget이 설정되지 않은 상태에서 동기화 함수를 호출하면 경고 로그가 출력됩니다.

5. **성능**: 주기적 동기화로 인한 성능 영향이 있을 수 있지만, 메시 변경이 감지된 경우에만 실행됩니다.

## 예시 시나리오

### 캐릭터 커스터마이징 시스템
```cpp
// 플레이어가 헤어 스타일을 변경할 때
void UCustomizationWidget::OnHairStyleChanged(USkeletalMesh* NewHairMesh)
{
    ACNox_Runner* Runner = GetOwningPlayerPawn<ACNox_Runner>();
    if (Runner)
    {
        // Runner의 헤어 메시 변경 (무조건 RenderTarget에 동기화됨)
        Runner->SetHairMesh(NewHairMesh);
    }
}
```

### 장비 시스템
```cpp
// 플레이어가 새로운 상의를 착용할 때
void UEquipmentSystem::EquipUpperBody(USkeletalMesh* NewUpperBodyMesh)
{
    ACNox_Runner* Runner = GetOwningPlayerPawn<ACNox_Runner>();
    if (Runner)
    {
        // Runner의 상의 메시 변경 (무조건 RenderTarget에 동기화됨)
        Runner->SetUpperBodyMesh(NewUpperBodyMesh);
    }
}
```

### 무기 장착 시스템
```cpp
// 플레이어가 무기를 장착할 때
void UWeaponSystem::EquipWeapon(USkeletalMesh* WeaponMesh, int32 SlotIndex)
{
    ACNox_Runner* Runner = GetOwningPlayerPawn<ACNox_Runner>();
    if (Runner)
    {
        if (SlotIndex == 1)
        {
            // 무기 슬롯1에 무기 메시 설정 (무조건 RenderTarget에 동기화됨)
            Runner->SetWeaponSlot1Mesh(WeaponMesh);
        }
        else if (SlotIndex == 2)
        {
            // 무기 슬롯2에 무기 메시 설정 (무조건 RenderTarget에 동기화됨)
            Runner->SetWeaponSlot2Mesh(WeaponMesh);
        }
    }
}
```

이 시스템을 사용하면 Runner와 RenderTarget 간의 메시 동기화를 **완전히 자동화**할 수 있으며, 캐릭터 커스터마이징이나 장비 시스템에서 매우 유용하게 활용할 수 있습니다. **한 번 설정하면 더 이상 신경 쓸 필요가 없습니다!** 