# 🎮 무량공처 (無量空處)

<aside>

**무량공처**는 Unreal Engine 5.5로 제작한 **멀티플레이 탑다운 슈팅 뱀서라이크(Vampire Survivors-like)** 프로젝트입니다.

*League of Legends*의 **집중포화** 모드를 레퍼런스로 삼아  
**빠른 전투 템포**, **랜덤 업그레이드 시스템**, **쾌적한 네트워크 경험**을 구현하는 것을 목표로 하였습니다.

- **개발 목표**
  - `Object Pool`을 활용한 액터 및 이펙트 관리 최적화
  - `Google Sheet API` 연동으로 밸런스 데이터 자동 반영
  - `Listen Server` 기반의 멀티플레이 환경 구축

</aside>

---

## ✅ 프로젝트 정보

- **장르**: 멀티플레이 탑다운 슈팅 / 뱀서라이크
- **개발 기간**: 2025.04.02 ~ 2025.04.30 (4주)
- **개발 인원**: 2명 (기획 + 프로그래밍)

---

## 📈 성과

- `Object Pool`, `Behavior Tree`, `Google Sheet API`, `멀티 세션 처리` 등 고급 기술 실전 적용
- 네트워크 동기화 문제 해결 경험 (`DOREPLIFETIME_CONDITION` 활용)
- 리팩토링 기반 구조 설계로 유지보수성 확보

---

## 👨‍💻 팀원 & 역할

| 이름 | 주요 담당 업무 |
|------|----------------|
| **XR** | - Google Sheet API 연동<br>- 전략 패턴 기반 스킬 시스템<br>- Object Pooling<br>- 업그레이드 UI<br>- 경험치 및 레벨업<br>- 플레이어/중간 보스 구현 |
| **XR** | - 세션 관리<br>- 적 AI 이동/공격<br>- 메인 보스 구현<br>- Flow Field 이동 설계 |

---

## 🛠 기술 스택

<aside>

- **엔진** : Unreal Engine 5.5  
- **언어** : C++  
- **Network** : Listen Server, `OnlineSubsystemSteam`  
- **Data 연동** : Google Sheet API (HTTP 기반)  
- **AI** : Behavior Tree, Flow Field Navigation  
- **패턴** : 전략 패턴  
- **최적화** : Object Pooling  
- **버전 관리** : Git (GitHub)

</aside>

---

## 🔧 주요 기능

- **스킬 시스템**: 전략 패턴 기반, 템플릿 스킬 구조
- **Object Pooling**: 스킬/적 생성 성능 최적화
- **AI 이동**: Flow Field + Behavior Tree 조합
- **업그레이드 시스템**: 레벨업 시 무작위 능력 선택 UI
- **세션 시스템**: Steam 기반 멀티 접속 처리
- **Google Sheet 연동**: 외부 밸런스 데이터 자동 반영

---

## 🧩 시스템 다이어그램

| 시스템 | 설명 |
|--------|------|
| 플레이어 입력 처리 | 입력 → 미션 처리 → UI까지 흐름 |
| 데이터 로딩 | Google Sheet → DataMap → 적용 |
| 레벨업 | 경험치 → 업그레이드 선택 UI → 능력 적용 |
| 스킬 선택 | 캐릭터 스폰 → 스킬 입력 및 실행 흐름 |
| 오브젝트 풀링 | 적/스킬 액터 재사용 관리 구조 |

{"https://github.com/user-attachments/assets/bc9d9c63-c47c-4f3b-a8b2-89cf549756b7"}

---

## 🖥 화면 구성

- 세션 접속/생성 화면
- 캐릭터 선택 UI
- 탑다운 전투 화면
- 업그레이드 카드 선택 UI
- 승리/패배 UI

> 📸 실제 인게임 캡처는 `/Assets/Screenshots` 폴더 참고

---

## 💬 회고

- 다양한 언리얼 기술을 실전에서 적용하며, 구조화된 개발 방식에 대한 감각을 익혔음  
- 멀티플레이 UI 동기화와 같은 실질적인 문제 해결 경험 축적  
- 추후엔 테스트 및 디버깅 프로세스를 정식 루틴에 포함하고 싶음

---

## 📚 레퍼런스

- Riot Games - *League of Legends: 집중포화 모드*

