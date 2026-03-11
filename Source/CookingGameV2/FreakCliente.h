#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FreakCliente.generated.h"

UCLASS()
class COOKINGGAMEV2_API AFreakCliente : public ACharacter
{
	GENERATED_BODY()

public:
	AFreakCliente();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Freak IA")
	AActor* PuntoMesa;

	UPROPERTY(EditAnywhere, Category = "Freak IA")
	float TiempoDeEspera = 3.0f;

private:
	FVector PuntoInicio;
	bool bYendoAMesa;
	bool bVolviendo;
	FTimerHandle TemporizadorEspera;

	void MoverAMesa();
	void IniciarEspera();
	void VolverAInicio();
	void ReiniciarLoop();
};